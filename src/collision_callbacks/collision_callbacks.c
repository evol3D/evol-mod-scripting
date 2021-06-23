#include "collision_callbacks.h"

void
init_collision_callbacks()
{
  if(Data.ecs_mod) {
    imports(Data.ecs_mod, (ECS, GameECS));

    if(GameECS) {
      Data.frameCollisionEnterListComponentID = GameECS->registerComponent("FrameCollisionEnterListComponent", sizeof(FrameCollisionEnterListComponent), EV_ALIGNOF(FrameCollisionEnterListComponent));
      Data.frameCollisionLeaveListComponentID = GameECS->registerComponent("FrameCollisionLeaveListComponent", sizeof(FrameCollisionLeaveListComponent), EV_ALIGNOF(FrameCollisionLeaveListComponent));

      GameECS->setOnAddTrigger("FrameCollisionEnterListComponentOnAdd", "FrameCollisionEnterListComponent", onAddEntitiesList);
      GameECS->setOnAddTrigger("FrameCollisionLeaveListComponentOnAdd", "FrameCollisionLeaveListComponent", onAddEntitiesList);

      GameECS->setOnRemoveTrigger("FrameCollisionEnterListComponentOnRemove", "FrameCollisionEnterListComponent", onRemoveEntitiesList);
      GameECS->setOnRemoveTrigger("FrameCollisionLeaveListComponentOnRemove", "FrameCollisionLeaveListComponent", onRemoveEntitiesList);

      GameECS->registerSystem("ScriptComponent,FrameCollisionEnterListComponent,"TAG_NAME(on_collisionenter), EV_ECS_PIPELINE_STAGE_UPDATE, ScriptCallbackOnCollisionEnterSystem, "ScriptCallbackOnCollisionEnterSystem");
      GameECS->registerSystem("ScriptComponent,FrameCollisionLeaveListComponent,"TAG_NAME(on_collisionleave), EV_ECS_PIPELINE_STAGE_UPDATE, ScriptCallbackOnCollisionLeaveSystem, "ScriptCallbackOnCollisionLeaveSystem");
    }
  }
}

void
deinit_collision_callbacks()
{
}

void
onAddEntitiesList(
    ECSQuery query)
{
  struct EntitiesList *list = ECS->getQueryColumn(query, sizeof(struct EntitiesList), 1);
  list->entities = vec_init(ECSEntityID);
}

void
onRemoveEntitiesList(
    ECSQuery query)
{
  struct EntitiesList *list = ECS->getQueryColumn(query, sizeof(struct EntitiesList), 1);
  vec_fini(list->entities);
}

void
ScriptCallbackOnCollisionEnterSystem(
    ECSQuery query)
{
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  struct EntitiesList *collisionEnterEntities = ECS->getQueryColumn(query, sizeof(struct EntitiesList), 2);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);

  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  ScriptContext ctx;

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ctx = Data.contexts[cmp.ctx_h];
    lua_getglobal(ctx.L, "Entities");
    ECSEntityID entt = enttIDs[i];
    vec(ECSEntityID) collEntts = collisionEnterEntities[i].entities;

    lua_pushinteger(ctx.L, entt);
    lua_gettable(ctx.L, -2); // Entities[entt]

    lua_pushvalue(ctx.L, -1);
    lua_setglobal(ctx.L, OBJECT_SELFREF); // this = Entities[entt]

    for(U32 j = 0; j < vec_len(collEntts); j++) {
      lua_pushstring(ctx.L, "on_collisionenter");
      lua_gettable(ctx.L, -2); // this.on_collisionenter

      lua_pushinteger(ctx.L, collEntts[j]);
      lua_gettable(ctx.L, -4); // Entities[collEntts[j]]

      if(ev_lua_pcall(ctx.L, 1, 0, 0)) {
        ev_log_error("%s", lua_tostring(ctx.L, -1));
        lua_pop(ctx.L, 1);
      }
    }
    vec_clear(collEntts);

    // Popping `Entities[entt]` & `Entities`
    lua_pop(ctx.L, 2);
  }
}

void
ScriptCallbackOnCollisionLeaveSystem(
    ECSQuery query)
{  
  ScriptComponent *scriptComponents = ECS->getQueryColumn(query, sizeof(ScriptComponent), 1);
  struct EntitiesList *collisionLeaveEntities = ECS->getQueryColumn(query, sizeof(struct EntitiesList), 2);
  ECSEntityID *enttIDs = ECS->getQueryEntities(query);

  U32 count = ECS->getQueryMatchCount(query);
  ScriptComponent cmp;
  ScriptContext ctx;

  for(U32 i = 0; i < count; ++i) {
    cmp = scriptComponents[i];
    ctx = Data.contexts[cmp.ctx_h];
    lua_getglobal(ctx.L, "Entities");
    ECSEntityID entt = enttIDs[i];
    vec(GameEntityID) collEntts = collisionLeaveEntities[i].entities;

    lua_pushinteger(ctx.L, entt);
    lua_gettable(ctx.L, -2); // Entities[entt]

    lua_pushvalue(ctx.L, -1);
    lua_setglobal(ctx.L, OBJECT_SELFREF); // this = Entities[entt]

    for(U32 j = 0; j < vec_len(collEntts); j++) {
      lua_pushstring(ctx.L, "on_collisionleave");
      lua_gettable(ctx.L, -2); // this.on_collisionenter

      lua_pushinteger(ctx.L, collEntts[j]);
      lua_gettable(ctx.L, -4); // Entities[collEntts[j]]

      if(ev_lua_pcall(ctx.L, 1, 0, 0)) {
        ev_log_error("%s", lua_tostring(ctx.L, -1));
        lua_pop(ctx.L, 1);
      }
    }
    vec_clear(collEntts);

    // Popping `Entities[entt]` & `Entities`
    lua_pop(ctx.L, 2);
  }
}

