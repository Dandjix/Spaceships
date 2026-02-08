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

inline void PhysicsEntity::registerInSpaceship(SpaceShip* space_ship)
{
    Entity::registerInSpaceship(space_ship);
    space_ship->physics_entities.insert(this);
    // if (!isJsonSerializable())
    //     std::cout << "inserted a non serializable ! " << std::endl;
}

inline void PhysicsEntity::unregisterInSpaceship(SpaceShip* space_ship, bool delete_when_done)
{
    Entity::unregisterInSpaceship(space_ship, delete_when_done);
    space_ship->physics_entities.add_to_erasing_queue(this);
}