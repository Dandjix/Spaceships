//
// Created by timon on 7/9/25.
//

#include "Behavior.h"

#include "PlayerBehavior.h"
#include "../entities/scripts/Entity.h"

class Entity;

Behavior * Behavior::fromJson(const nlohmann::basic_json<> &value) {
    if (value == "player_behavior") {
        return new PlayerBehavior();
    }
    return nullptr;
}
