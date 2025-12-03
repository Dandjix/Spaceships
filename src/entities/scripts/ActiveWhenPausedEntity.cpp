//
// Created by timon on 12/3/25.
//

#include "ActiveWhenPausedEntity.h"

inline void ActiveWhenPausedEntity::registerInSpaceship(SpaceShip *space_ship) {
    ShortLivedEntity::registerInSpaceship(space_ship);
    space_ship->active_when_paused_entities.insert(this);
}

inline void ActiveWhenPausedEntity::unregisterInSpacehip(SpaceShip *space_ship) {
    ShortLivedEntity::unregisterInSpacehip(space_ship);
    space_ship->active_when_paused_entities.erase(this);
}