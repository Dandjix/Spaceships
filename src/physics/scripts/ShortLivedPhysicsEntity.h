#pragma once
#include "PhysicsEntity.h"
//
// Created by timon on 2/8/26.


/**
 * Yeah, I know, this sucks but i cant figure out for the life of me how to make ShortLivedEntity an interface, soo ....
 */
class ShortLivedPhysicsEntity : public PhysicsEntity {
public:
    ShortLivedPhysicsEntity(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape)
        : PhysicsEntity(position, angle, shape) {
    }

    nlohmann::json toJson() final {
        throw std::invalid_argument("Tried to convert a short lived (physics) object to JSON : this is illegal");
    }

    bool isJsonSerializable() final {
        return false;
    }

    constexpr std::string getJsonType() final {
        // throw std::invalid_argument("Tried to get the json type of a short lived object : this is illegal"); //can't throw in a constexpr sadly
        return "short_lived_physics_entity";
    }

    void finalizeJsonDeserialization(const GameState::transientGameState &transient_game_state) final {
    }
};
