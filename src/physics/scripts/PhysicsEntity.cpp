//
// Created by timon on 11/18/25.
//

#include "PhysicsEntity.h"

#include "entities/scripts/Entity.h"
#include "../shapes/PhysicsShape.h"
#include "entities/toggleables/door/DoorPanel.h"
#include "spaceships/SpaceShip.h"

PhysicsEntity::~PhysicsEntity() {
    delete shape;
}

inline void PhysicsEntity::registerInInstance(Instances::Instance *world_instance)
{
    Entity::registerInInstance(world_instance);
    world_instance->physics_entities.insert(this);
    // if (!isJsonSerializable())
    //     std::cout << "inserted a non serializable ! " << std::endl;
}

inline void PhysicsEntity::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done)
{
    Entity::unregisterInInstance(world_instance, delete_when_done);
    world_instance->physics_entities.add_to_erasing_queue(this);
}