#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <evol/common/ev_log.h>
#include <evol/utils/lua_evutils.h>

#include <hashmap.h>

#include <luajit.h>
#include <lauxlib.h>

#define TYPE_MODULE evmod_ecs
#define NAMESPACE_MODULE evmod_ecs
#include <evol/meta/type_import.h>
#include <evol/meta/namespace_import.h>

#define OBJECT_SELFREF "this"

#define TAG_NAME(x) EV_STRINGIZE(EV_CONCAT(ScriptCB,x))
#define SCRIPT_TAG(x) EV_CONCAT(SCRIPT_TAG_,x)
#define EV_SCRIPT_CALLBACK(x) EV_CONCAT(EV_SCRIPT_CALLBACK_,x)

#define SCRIPT_CALLBACK_FUNCTIONS() \
  SCRIPT_OP(on_init)        \
  SCRIPT_OP(on_update)      \
  SCRIPT_OP(on_fixedupdate) \

typedef enum {
#define SCRIPT_OP(x) SCRIPT_TAG(x),
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP
  SCRIPT_TAG(COUNT)
} ScriptCallbackTags;

typedef enum {
#define SCRIPT_OP(x) EV_SCRIPT_CALLBACK(x) = 1 << SCRIPT_TAG(x),
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP
} ScriptCallbackFlagBits;
typedef ScriptCallbackFlagBits ScriptCallbackFlags;

struct {
  lua_State *L;
  evolmodule_t ecs_mod;
  ECSEntityID scriptComponentID;
  ECSEntityID scriptTagIDs[SCRIPT_TAG(COUNT)];
  struct hashmap *scripts;
} Data;

typedef struct {
  evstring script;
  ScriptCallbackFlags cbFlags;
} ScriptComponent;
typedef struct {
  evstring id_str;
  ScriptHandle component;
} ScriptEntry;

static_assert(sizeof(ScriptType) == sizeof(lua_Integer), "ScriptType is not the same size as lua_Integer");

void 
ScriptCallbackOnUpdateSystem(
    ECSQuery query)
{
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);
  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  lua_getglobal(Data.L, "Entities");

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ECSEntityID entt = enttIDs[i];
    lua_pushinteger(Data.L, entt);
    lua_gettable(Data.L, -2);

    // TODO find a cleaner way that setting then getting
    lua_setglobal(Data.L, OBJECT_SELFREF);
    lua_getglobal(Data.L, OBJECT_SELFREF);

    // Get `Entities[entt]["on_update"]
    lua_pushstring(Data.L, "on_update");
    lua_gettable(Data.L, -2);
    if(lua_pcall(Data.L, 0, 0, 0)) {
      ev_log_error("%s", lua_tostring(Data.L, -1));
      lua_pop(Data.L, 1);
    }

    // Popping `Entities[entt]`
    lua_pop(Data.L, 1);
  }
  // Popping `Entities`
  lua_pop(Data.L, 1);
}

void 
ScriptCallbackOnFixedUpdateSystem(
    ECSQuery query)
{
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);
  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  lua_getglobal(Data.L, "Entities");

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ECSEntityID entt = enttIDs[i];
    lua_pushinteger(Data.L, entt);
    lua_gettable(Data.L, -2);

    // TODO find a cleaner way that setting then getting
    lua_setglobal(Data.L, OBJECT_SELFREF);
    lua_getglobal(Data.L, OBJECT_SELFREF);

    // Get `Entities[entt]["on_fixedupdate"]
    lua_pushstring(Data.L, "on_fixedupdate");
    lua_gettable(Data.L, -2);
    if(lua_pcall(Data.L, 0, 0, 0)) {
      ev_log_error("%s", lua_tostring(Data.L, -1));
      lua_pop(Data.L, 1);
    }

    // Popping `Entities[entt]`
    lua_pop(Data.L, 1);
  }
  // Popping `Entities`
  lua_pop(Data.L, 1);
}

void
_ev_script_addtoentity(
    ECSEntityID entt, 
    ScriptHandle handle)
{
  ECS->addComponent(entt, Data.scriptComponentID, sizeof(ScriptComponent), handle);
  ScriptComponent *p_cmp = (ScriptComponent *)handle;

#define SCRIPT_OP(x) \
  if(p_cmp->cbFlags & EV_SCRIPT_CALLBACK(x)) { \
    ECS->addTag(entt, Data.scriptTagIDs[SCRIPT_TAG(x)]); \
  }
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP

  lua_getglobal(Data.L, "Entity");
  lua_getfield(Data.L, -1, "new");
  lua_getglobal(Data.L, "Entity");
  lua_pushinteger(Data.L, entt);
  if(lua_pcall(Data.L, 2, 1, 0)) {
    ev_log_error("%s", lua_tostring(Data.L, -1));
    lua_pop(Data.L, 1);
  }

  lua_setglobal(Data.L, OBJECT_SELFREF);
  lua_pop(Data.L, 1);

  ScriptComponent * cmp = (ScriptComponent *)handle;
  ev_lua_runstring(Data.L, cmp->script);

}

ScriptHandle 
_ev_script_new(
    CONST_STR id, 
    CONST_STR scriptString)
{
  ScriptComponent *cmp = malloc(sizeof(ScriptComponent)); 
  cmp->script = evstring_new(scriptString);

  lua_newtable(Data.L);
  lua_setglobal(Data.L, OBJECT_SELFREF);

  ev_lua_runstring(Data.L, scriptString);

  lua_getglobal(Data.L, OBJECT_SELFREF);
  DEBUG_ASSERT(lua_istable(Data.L, -1));

  cmp->cbFlags = 0;

#define SCRIPT_OP(x) {                       \
    lua_pushstring(Data.L, EV_STRINGIZE(x)); \
    lua_gettable(Data.L, -2);                \
    if(lua_isfunction(Data.L, -1)) {         \
      cmp->cbFlags |= EV_SCRIPT_CALLBACK(x); \
    }                                        \
    lua_pop(Data.L, 1);                      \
  }
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP

  lua_pop(Data.L, 1); // matches: `lua_getglobal(Dat.L, "object");`

  // TODO Optionally, set `object` to an empty table again
  // Will this help GC?
  /* lua_newtable(Data.L); */
  /* lua_setglobal(Data.L, "object"); */

  ScriptHandle handle = (ScriptHandle)cmp;
  ScriptEntry entry = {
    .id_str = evstring_new(id),
    .component = handle,
  };
  hashmap_set(Data.scripts, &entry);
  return handle;
}

U64
scriptentry_hash(
    const PTR data,
    U64 seed0,
    U64 seed1)
{
  const ScriptEntry *entry = data;
  return hashmap_murmur(entry->id_str, evstring_len(entry->id_str), seed0, seed1);
}

I32 
scriptentry_compare(
  const PTR data1, 
  const PTR data2, 
  PTR _udata)
{
  EV_UNUSED_PARAM(_udata);
  const ScriptEntry *entry1 = data1;
  const ScriptEntry *entry2 = data2;

  return evstring_cmp(entry1->id_str, entry2->id_str);
}

void
_ev_scriptinterface_loadapi(
    CONST_STR file_path)
{
  ev_lua_runfile(Data.L, file_path);
}



EV_CONSTRUCTOR
{
  Data.ecs_mod = NULL;
  Data.L = ev_lua_newState(true);
  Data.scripts = hashmap_new(sizeof(ScriptEntry), 16, 0, 0, scriptentry_hash, scriptentry_compare, NULL);

  _ev_scriptinterface_loadapi("subprojects/evmod_script/evol_api.lua");

  return 0;
}

bool
script_delete(
    const PTR data,
    PTR _udata)
{
  EV_UNUSED_PARAM(_udata);
  ScriptEntry *entry = data;
  evstring_free(entry->id_str);
  evstring_free(((ScriptComponent*)entry->component)->script);
  free(entry->component);
  return true;
}

void 
clear_script_entries()
{
  hashmap_scan(Data.scripts, script_delete, NULL);
}

EV_DESTRUCTOR 
{
  ev_lua_destroyState(&Data.L);
  if(Data.ecs_mod != NULL) {
    evol_unloadmodule(Data.ecs_mod);
  }
  clear_script_entries();
  hashmap_free(Data.scripts);
  return 0;
}

void
_ev_script_initecs()
{
  Data.ecs_mod = evol_loadmodule("ecs");
  IMPORT_NAMESPACE(ECS, Data.ecs_mod);
  Data.scriptComponentID = ECS->registerComponent("ScriptComponent", sizeof(ScriptComponent), EV_ALIGNOF(ScriptComponent));

#define SCRIPT_OP(x) Data.scriptTagIDs[SCRIPT_TAG(x)] = ECS->registerTag(TAG_NAME(x));
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP

  ECS->registerSystem("ScriptComponent,"TAG_NAME(on_update), EV_ECS_PIPELINE_STAGE_UPDATE, ScriptCallbackOnUpdateSystem, "ScriptCallbackOnUpdateSystem");
  ECSSystemID fixedUpdateSystem = ECS->registerSystem("ScriptComponent,"TAG_NAME(on_fixedupdate), EV_ECS_PIPELINE_STAGE_UPDATE, ScriptCallbackOnFixedUpdateSystem, "ScriptCallbackOnFixedUpdateSystem");
  ECS->setSystemRate(fixedUpdateSystem, 60.0);
}

ScriptType
_ev_scriptinterface_addtype(
    CONST_STR typename,
    U32 size)
{
  return luaA_type_add(Data.L, typename, size);
}

ScriptType
_ev_scriptinterface_gettype(
    CONST_STR typename)
{
  return luaA_type_find(Data.L, typename);
}

ScriptType
_ev_scriptinterface_addstruct(
    CONST_STR typename,
    U32 size,
    U32 member_count,
    ScriptStructMember *members)
{
  ScriptType sType = luaA_type_add(Data.L, typename, size);
  luaA_struct_type(Data.L, sType);

  for(U32 i = 0; i < member_count; ++i) {
    luaA_struct_member_type(Data.L, sType, members[i].name, members[i].type, members[i].offset);
  }

  return sType;
}

#include <misc/func_wrappers.h>

void
_ev_scriptinterface_addfunction(
    FN_PTR func, 
    CONST_STR func_name, 
    ScriptType rettype, 
    U32 arg_count,
    ScriptType *args)
{
  static ScriptType voidType = 0;
  if(voidType == 0) {
    voidType = _ev_scriptinterface_gettype("void");
  }

  luaA_Func auto_func;
  if(rettype == voidType) {
    auto_func = luaA_wrap_functions_void[arg_count];
  } else {
    auto_func = luaA_wrap_functions[arg_count];
  }

  luaA_function_register_type(Data.L, func, auto_func, func_name, rettype, arg_count, args);
}

EV_BINDINGS
{
  EV_NS_BIND_FN(Script, new, _ev_script_new);
  EV_NS_BIND_FN(Script, addToEntity, _ev_script_addtoentity);
  EV_NS_BIND_FN(Script, initECS, _ev_script_initecs);

  EV_NS_BIND_FN(ScriptInterface, addFunction, _ev_scriptinterface_addfunction);
  EV_NS_BIND_FN(ScriptInterface, addType, _ev_scriptinterface_addtype);
  EV_NS_BIND_FN(ScriptInterface, getType, _ev_scriptinterface_gettype);
  EV_NS_BIND_FN(ScriptInterface, addStruct, _ev_scriptinterface_addstruct);

  EV_NS_BIND_FN(ScriptInterface, loadAPI, _ev_scriptinterface_loadapi);
}
