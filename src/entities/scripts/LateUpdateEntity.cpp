//
// Created by timon on 12/3/25.
//

#include "LateUpdateEntity.h"

#include "spaceships/SpaceShip.h"

LateUpdateEntity::LateUpdateEntity(const Vector2Int &position, const std::optional<float> &angle): Entity(position, angle) {
}

void LateUpdateEntity::registerInInstance(Instances::Instance *world_instance) {
    Entity::registerInInstance(world_instance);
    world_instance->late_update_entities.insert(this);
}

void LateUpdateEntity::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) {
    world_instance->late_update_entities.add_to_erasing_queue(this,false);
    Entity::unregisterInInstance(world_instance, delete_when_done);
}
