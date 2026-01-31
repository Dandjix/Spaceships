//
// Created by timon on 11/18/25.
//

#include "PhysicsEntity.h"

#include "entities/scripts/Entity.h"
#include "shapes/PhysicsShape.h"
#include "spaceships/SpaceShip.h"

PhysicsEntity::~PhysicsEntity() {
    delete shape;
}

inline void PhysicsEntity::registerInSpaceship(SpaceShip* space_ship)
{
    Entity::registerInSpaceship(space_ship);
    space_ship->physics_entities.insert(this);
}

inline void PhysicsEntity::unregisterInSpacehip(SpaceShip* space_ship)
{
    Entity::unregisterInSpacehip(space_ship);
    space_ship->physics_entities.erase(this);
}