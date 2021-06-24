#define EV_MODULE_DEFINE
#include <evol/evolmod.h>

#include <script_common.h>
#include "collision_callbacks/collision_callbacks.h"

#define IMPORT_MODULE evmod_game
#include <evol/meta/module_import.h>

struct ScriptModuleData Data;

void
scripthandle_free(
    ScriptHandle handle) 
{
  ScriptComponent *cmp = (ScriptComponent*)handle;
  evstring_free(cmp->script);
  free(cmp);
}

HashmapDefine(evstring, ScriptHandle, evstring_free, scripthandle_free)

static_assert(sizeof(ScriptType) == sizeof(lua_Integer), "ScriptType is not the same size as lua_Integer");

void 
ScriptCallbackOnUpdateSystem(
    ECSQuery query)
{
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);
  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  ScriptContext ctx;

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ctx = Data.contexts[cmp.ctx_h];
    lua_getglobal(ctx.L, "Entities");
    ECSEntityID entt = enttIDs[i];
    lua_pushinteger(ctx.L, entt);
    lua_gettable(ctx.L, -2);

    lua_pushvalue(ctx.L, -1);
    lua_setglobal(ctx.L, OBJECT_SELFREF);

    // Get `Entities[entt]["on_update"]
    lua_pushstring(ctx.L, "on_update");
    lua_gettable(ctx.L, -2);

    if(ev_lua_pcall(ctx.L, 0, 0, 0)) {
      ev_log_error("%s", lua_tostring(ctx.L, -1));
      lua_pop(ctx.L, 1);
    }

    // Popping `Entities[entt]` & `Entities`
    lua_pop(ctx.L, 2);
  }
}

void 
ScriptCallbackOnFixedUpdateSystem(
    ECSQuery query)
{
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);
  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  ScriptContext ctx;

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ctx = Data.contexts[cmp.ctx_h];
    lua_getglobal(ctx.L, "Entities");
    ECSEntityID entt = enttIDs[i];
    lua_pushinteger(ctx.L, entt);
    lua_gettable(ctx.L, -2);

    lua_pushvalue(ctx.L, -1);
    lua_setglobal(ctx.L, OBJECT_SELFREF);

    // Get `Entities[entt]["on_fixedupdate"]
    lua_pushstring(ctx.L, "on_fixedupdate");
    lua_gettable(ctx.L, -2);

    if(ev_lua_pcall(ctx.L, 0, 0, 0)) {
      ev_log_error("%s", lua_tostring(ctx.L, -1));
      lua_pop(ctx.L, 1);
    }

    // Popping `Entities[entt]` & `Entities`
    lua_pop(ctx.L, 2);
  }
}

void
_ev_script_addtoentity(
    GameScene scene,
    GameEntityID entt,
    ScriptHandle handle)
{
  ScriptComponent *p_cmp = (ScriptComponent *)handle;
  ScriptContextHandle ctx_h = Scene->getScriptContext(scene);
  ScriptContext ctx = Data.contexts[ctx_h];

  ECSGameWorldHandle world = Scene->getECSWorld(scene);
  GameECS->setComponent(world, entt, Data.scriptComponentID, &(ScriptComponent) {
      .script = p_cmp->script,
      .cbFlags = p_cmp->cbFlags,
      .ctx_h = ctx_h
  });

#define SCRIPT_OP(x) \
  if(p_cmp->cbFlags & EV_SCRIPT_CALLBACK(x)) { \
    GameECS->addTag(world, entt, Data.scriptTagIDs[SCRIPT_TAG(x)]); \
  }
  SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP

  if(p_cmp->cbFlags & EV_SCRIPT_CALLBACK(on_collisionenter)) {
    GameECS->addComponent(world, entt, Data.frameCollisionEnterListComponentID);
  }
  if(p_cmp->cbFlags & EV_SCRIPT_CALLBACK(on_collisionleave)) {
    GameECS->addComponent(world, entt, Data.frameCollisionLeaveListComponentID);
  }

  lua_getglobal(ctx.L, "Entity");
  lua_getfield(ctx.L, -1, "new");
  lua_getglobal(ctx.L, "Entity");
  lua_pushinteger(ctx.L, entt);
  if(ev_lua_pcall(ctx.L, 2, 1, 0)) {
    ev_log_error("%s", lua_tostring(ctx.L, -1));
    lua_pop(ctx.L, 1);
  }

  lua_setglobal(ctx.L, OBJECT_SELFREF);
  lua_pop(ctx.L, 1);

  ScriptComponent * cmp = (ScriptComponent *)handle;
  ev_lua_runstring(ctx.L, cmp->script);

  if(p_cmp->cbFlags & EV_SCRIPT_CALLBACK(on_init)) {
    lua_getglobal(ctx.L, OBJECT_SELFREF);
    lua_pushstring(ctx.L, "on_init");
    lua_gettable(ctx.L, -2);

    if(ev_lua_pcall(ctx.L, 0, 0, 0)) {
      ev_log_error("%s", lua_tostring(ctx.L, -1));
      lua_pop(ctx.L, 1);
    }

    lua_pop(ctx.L, 1);
  }
}

ScriptHandle 
_ev_script_new(
    CONST_STR id, 
    CONST_STR scriptString)
{
  ScriptComponent *cmp = malloc(sizeof(ScriptComponent)); 
  cmp->ctx_h = -1;
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
  Hashmap(evstring, ScriptHandle).push(Data.scripts, evstring_new(id), handle);
  return handle;
}

void
_ev_scriptinterface_loadapi(
    ScriptContextHandle ctx_h,
    CONST_STR file_path)
{
  ev_lua_runfile(Data.contexts[ctx_h].L, file_path);
}

void
scriptcontext_destr(
    void *data)
{
  ScriptContext *ctx = (ScriptContext *)data;
  if(ctx->L) {
    ev_lua_destroyState(&(ctx->L));
  }
}

void
ev_scriptmod_scriptapi_loader(
    EVNS_ScriptInterface *_ScriptInterface,
    ScriptContextHandle ctx_h)
{
  EV_UNUSED_PARAMS(_ScriptInterface);
  _ev_scriptinterface_loadapi(ctx_h, "subprojects/evmod_script/evol_api.lua");
}

EV_CONSTRUCTOR
{
  ev_log_trace("[evmod_script] Constructor");

  Data.api_loaders = vec_init(ScriptAPILoaderFN);
  Data.contexts = vec_init(ScriptContext, NULL, scriptcontext_destr);

  ScriptAPILoaderFN fn = (ScriptAPILoaderFN)ev_scriptmod_scriptapi_loader;
  vec_push((vec_t*)&Data.api_loaders, &fn);

  Data.L = ev_lua_newState(true);
  ev_log_trace("[evmod_script] Loading Script API");

  Data.scripts = Hashmap(evstring, ScriptHandle).new();

  evolmodule_t game_mod = evol_loadmodule_weak("game");
  if(game_mod) {
    ev_log_trace("[evmod_script] Found Game module. Importing namespaces (Object, Scene)");
    imports(game_mod, (Object, Scene));
  }

  Data.ecs_mod = evol_loadmodule("ecs");
  if(Data.ecs_mod) {
    ev_log_trace("[evmod_script] Found ECS module. Importing namespaces (ECS, GameECS)");
    imports(Data.ecs_mod, (ECS, GameECS));

    if(GameECS) {
      ev_log_trace("[evmod_script] Registering component { ScriptComponent }");
      Data.scriptComponentID = GameECS->registerComponent("ScriptComponent", sizeof(ScriptComponent), EV_ALIGNOF(ScriptComponent));

#define SCRIPT_OP(x) Data.scriptTagIDs[SCRIPT_TAG(x)] = GameECS->registerTag(TAG_NAME(x));
      SCRIPT_CALLBACK_FUNCTIONS()
#undef SCRIPT_OP

      Data.onUpdateQuery = GameECS->registerQuery("ScriptComponent,"TAG_NAME(on_update));
      GameSystemID fixedUpdateSystem = GameECS->registerSystem("ScriptComponent,"TAG_NAME(on_fixedupdate), EV_ECS_PIPELINE_STAGE_UPDATE, ScriptCallbackOnFixedUpdateSystem, "ScriptCallbackOnFixedUpdateSystem");
      GameECS->setSystemRate(fixedUpdateSystem, 60.0);
    }
  }

  init_collision_callbacks();

  return 0;
}

EV_DESTRUCTOR 
{
  deinit_collision_callbacks();

  ev_lua_destroyState(&Data.L);
  if(Data.ecs_mod != NULL) {
    evol_unloadmodule(Data.ecs_mod);
  }
  Hashmap(evstring, ScriptHandle).free(Data.scripts);

  vec_fini(Data.api_loaders);
  vec_fini(Data.contexts);
  return 0;
}

I32
ev_script_progress(
    F32 delta_time)
{
  EV_UNUSED_PARAMS(delta_time);
  ECSGameWorldHandle world_handle = Scene->getECSWorld(0);
  GameECS->runSystem(world_handle, Data.onUpdateQuery, ScriptCallbackOnUpdateSystem);

  return 0;
}

vec(U64)*
_ev_script_getcollisionenterlist(
    GameScene scene,
    U64 entt)
{
  if(Object->hasComponent(scene, entt, Data.frameCollisionEnterListComponentID)) {
    struct EntitiesList *list = Object->getComponent(scene, entt, Data.frameCollisionEnterListComponentID);
    return &(list->entities);
  }
  return NULL;
}

vec(U64)*
_ev_script_getcollisionleavelist(
    GameScene scene,
    U64 entt)
{
  if(Object->hasComponent(scene, entt, Data.frameCollisionLeaveListComponentID)) {
    struct EntitiesList *list = Object->getComponent(scene, entt, Data.frameCollisionLeaveListComponentID);
    return &(list->entities);
  }
  return NULL;
}

ScriptType
_ev_scriptinterface_addtype(
    ScriptContextHandle ctx_h,
    CONST_STR typename,
    U32 size)
{
  return luaA_type_add(Data.contexts[ctx_h].L, typename, size);
}

ScriptType
_ev_scriptinterface_gettype(
    ScriptContextHandle ctx_h,
    CONST_STR typename)
{
  return luaA_type_find(Data.contexts[ctx_h].L, typename);
}

ScriptType
_ev_scriptinterface_addstruct(
    ScriptContextHandle ctx_h,
    CONST_STR typename,
    U32 size,
    U32 member_count,
    ScriptStructMember *members)
{
  ScriptContext ctx = Data.contexts[ctx_h];
  ScriptType sType = luaA_type_add(ctx.L, typename, size);
  luaA_struct_type(ctx.L, sType);

  for(U32 i = 0; i < member_count; ++i) {
    luaA_struct_member_type(ctx.L, sType, members[i].name, members[i].type, members[i].offset);
  }

  return sType;
}

#include <misc/func_wrappers.h>

void
_ev_scriptinterface_addfunction(
    ScriptContextHandle ctx_h,
    FN_PTR func, 
    CONST_STR func_name, 
    ScriptType rettype, 
    U32 arg_count,
    ScriptType *args)
{
  ScriptContext ctx = Data.contexts[ctx_h];
  static ScriptType voidType = 0;
  if(voidType == 0) {
    voidType = _ev_scriptinterface_gettype(ctx_h, "void");
  }

  luaA_Func auto_func;
  if(rettype == voidType) {
    auto_func = luaA_wrap_functions_void[arg_count];
  } else {
    auto_func = luaA_wrap_functions[arg_count];
  }

  luaA_function_register_type(ctx.L, (PTR)func, auto_func, func_name, rettype, arg_count, (luaA_Type*)args);
}

void
ev_scriptinterface_registerapiloadfn(
    ScriptAPILoaderFN func)
{
  vec_push((vec_t*)&Data.api_loaders, &func);
  for(size_t ctx_h = 0; ctx_h < vec_len(Data.contexts); ctx_h++) {
    func(&SELF(ScriptInterface), ctx_h);
  }
}

ScriptContextHandle
ev_scriptcontext_newcontext()
{
  ScriptContext ctx = {
    .L = ev_lua_newState(true)
  };

  if(ctx.L == NULL) {
    ev_log_error("[evscript_mod] Failed to create a new ScriptContext. Lua State creation failed.");
    return -1;
  }

  ScriptContextHandle ctx_h = vec_push((vec_t*)&Data.contexts, &ctx);

  for(size_t i = 0; i < vec_len(Data.api_loaders); i++) {
    Data.api_loaders[i](&SELF(ScriptInterface), ctx_h);
  }

  return ctx_h;
}

ScriptContextHandle
ev_scriptcontext_invalidhandle()
{
  return INVALID_CONTEXT_HANDLE;
}

void
ev_scriptcontext_destroycontext(
    ScriptContextHandle handle)
{
  if(handle == INVALID_CONTEXT_HANDLE) {
    return;
  }
  ScriptContext *ctx = &Data.contexts[handle];
  if(ctx->L == NULL) {
    return;
  }
  EvLuaUtilsResult res = ev_lua_destroyState(&ctx->L);

  if(res != EV_LUAUTILS_SUCCESS) {
    ev_log_error("[evscript_mod] Failed to destroy ScriptContext with handle { %llu }. Reason: %s", handle, EvLuaUtilsResultStrings[res]);
  }
}

EV_BINDINGS
{
  EV_NS_BIND_FN(Script, new, _ev_script_new);
  EV_NS_BIND_FN(Script, progress, ev_script_progress);
  EV_NS_BIND_FN(Script, addToEntity, _ev_script_addtoentity);
  EV_NS_BIND_FN(Script, getCollisionEnterList, _ev_script_getcollisionenterlist);
  EV_NS_BIND_FN(Script, getCollisionLeaveList, _ev_script_getcollisionleavelist);

  EV_NS_BIND_FN(ScriptInterface, addFunction, _ev_scriptinterface_addfunction);
  EV_NS_BIND_FN(ScriptInterface, addType, _ev_scriptinterface_addtype);
  EV_NS_BIND_FN(ScriptInterface, getType, _ev_scriptinterface_gettype);
  EV_NS_BIND_FN(ScriptInterface, addStruct, _ev_scriptinterface_addstruct);
  EV_NS_BIND_FN(ScriptInterface, loadAPI, _ev_scriptinterface_loadapi);
  EV_NS_BIND_FN(ScriptInterface, registerAPILoadFn, ev_scriptinterface_registerapiloadfn);

  EV_NS_BIND_FN(ScriptContext, invalidHandle, ev_scriptcontext_invalidhandle);
  EV_NS_BIND_FN(ScriptContext, newContext, ev_scriptcontext_newcontext);
  EV_NS_BIND_FN(ScriptContext, destroyContext, ev_scriptcontext_destroycontext);
}
