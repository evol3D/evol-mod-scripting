#pragma once

#define IMPORT_MODULE evmod_ecs
#include <evol/meta/module_import.h>

#if !defined(SCRIPT_MOD_TYPES_DEFINED)
#define SCRIPT_MOD_TYPES_DEFINED
#define TYPE_MODULE evmod_script
#include <evol/meta/type_import.h>
#endif

#include <evol/common/ev_log.h>
#include <evol/utils/lua_evutils.h>

#include <luajit.h>
#include <lauxlib.h>

#define INVALID_CONTEXT_HANDLE (~0ull)

#define SCRIPT_CALLBACK_FUNCTIONS() \
  SCRIPT_OP(on_init)           \
  SCRIPT_OP(on_update)         \
  SCRIPT_OP(on_fixedupdate)    \
  SCRIPT_OP(on_collisionenter) \
  SCRIPT_OP(on_collisionleave)

#define TAG_NAME(x) EV_STRINGIZE(EV_CONCAT(ScriptCB,x))
#define SCRIPT_TAG(x) EV_CONCAT(SCRIPT_TAG_,x)
#define EV_SCRIPT_CALLBACK(x) EV_CONCAT(EV_SCRIPT_CALLBACK_,x)

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

typedef struct {
  lua_State *L;
} ScriptContext;

typedef struct {
  evstring script;
  ScriptCallbackFlags cbFlags;
  ScriptContextHandle ctx_h;
} ScriptComponent;

struct ScriptModuleData {
  lua_State *L;
  evolmodule_t ecs_mod;
  GameComponentID scriptComponentID;
  GameTagID scriptTagIDs[SCRIPT_TAG(COUNT)];

  GameComponentID frameCollisionEnterListComponentID;
  GameComponentID frameCollisionLeaveListComponentID;

  GameQueryID onUpdateQuery;

  Map(evstring, ScriptHandle) scripts;

  vec(ScriptAPILoaderFN) api_loaders;
  vec(ScriptContext) contexts;
};

extern struct ScriptModuleData Data;

#define OBJECT_SELFREF "this"
