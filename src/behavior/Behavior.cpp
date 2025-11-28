//
// Created by timon on 7/9/25.
//

#include "Behavior.h"

#include "PlayerBehavior.h"
#include "../entities/Entity.h"

class Entity;

Behavior * Behavior::fromJson(const nlohmann::basic_json<> &value) {
    if (value == "player") {
        return new PlayerBehavior();
    }
}
