//
// Created by timon on 12/3/25.
//

#include "ActiveWhenPausedEntity.h"

#include "spaceships/SpaceShip.h"

inline void ActiveWhenPausedEntity::registerInInstance(Instances::Instance *world_instance) {
    ShortLivedEntity::registerInInstance(world_instance);
    world_instance->active_when_paused_entities.insert(this);
}

inline void ActiveWhenPausedEntity::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) {
    world_instance->active_when_paused_entities.add_to_erasing_queue(this,false);
    ShortLivedEntity::unregisterInInstance(world_instance, delete_when_done);
}