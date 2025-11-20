//
// Created by timon on 11/18/25.
//

#include "PhysicsEntity.h"

void PhysicsEntity::physicsUpdate(const PhysicsUpdateContext& context)
{
    std::cout << "Physics pos = "
          << this->position.x << ", " << this->position.y << "\n";
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