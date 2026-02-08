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

void LateUpdateEntity::unregisterInSpaceship(SpaceShip *space_ship, bool delete_when_done) {
    space_ship->late_update_entities.add_to_erasing_queue(this,false);
    Entity::unregisterInSpaceship(space_ship, delete_when_done);
}
