//
// Created by timon on 12/3/25.
//

#include "ActiveWhenPausedEntity.h"

#include "spaceships/SpaceShip.h"

inline void ActiveWhenPausedEntity::registerInSpaceship(SpaceShip *space_ship) {
    ShortLivedEntity::registerInSpaceship(space_ship);
    space_ship->active_when_paused_entities.insert(this);
}

inline void ActiveWhenPausedEntity::unregisterInSpaceship(SpaceShip *space_ship, bool delete_when_done) {
    space_ship->active_when_paused_entities.add_to_erasing_queue(this,false);
    ShortLivedEntity::unregisterInSpaceship(space_ship, delete_when_done);
}