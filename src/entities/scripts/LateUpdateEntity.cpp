//
// Created by timon on 12/3/25.
//

#include "LateUpdateEntity.h"

#include "spaceships/SpaceShip.h"

LateUpdateEntity::LateUpdateEntity(const Vector2Int &position, const std::optional<float> &angle): Entity(position, angle) {
}

void LateUpdateEntity::registerInSpaceship(SpaceShip *space_ship) {
    Entity::registerInSpaceship(space_ship);
    space_ship->late_update_entities.insert(this);
}

void LateUpdateEntity::unregisterInSpacehip(SpaceShip *space_ship) {
    Entity::unregisterInSpacehip(space_ship);
    space_ship->late_update_entities.erase(this);
}
