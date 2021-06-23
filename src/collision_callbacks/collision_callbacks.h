#pragma once

#include <script_common.h>

// TODO This is currently being used by both the physics module and the 
// scripting module. One solution would be to use the Event System for 
// dispatching collision events. However, that would mean that collision
// callbacks will be delayed by one frame.
// Another solution is to expose the ability for modules to create their own
// scripting callback functions. This seems a bit more plausible but it's not
// obvious whether it's worth it.
struct EntitiesList {
  vec(GameEntityID) entities;
};
typedef struct EntitiesList FrameCollisionEnterListComponent;
typedef struct EntitiesList FrameCollisionLeaveListComponent;

void
onAddEntitiesList(
    ECSQuery query);

// For some reason, this doesn't seem to get called when the ECS module is
// destructed. I'm assuming it doesn't matter much since the destruction of
// the module should free all of its allocated memory. However, ECS->deleteEntity
// seems to trigger the OnRemove event.
void
onRemoveEntitiesList(
    ECSQuery query);

void
ScriptCallbackOnCollisionEnterSystem(
    ECSQuery query);

void
ScriptCallbackOnCollisionLeaveSystem(
    ECSQuery query);

void
init_collision_callbacks();

void
deinit_collision_callbacks();
