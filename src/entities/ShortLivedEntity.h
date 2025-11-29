//
// Created by timon on 11/28/25.


#pragma once
#include "Entity.h"

/**
 * By short lived entity I mean an entity that does not implement JSON serialization, such as the ship editor entities.
 */
class ShortLivedEntity : public Entity {
public:
    ShortLivedEntity(const Vector2Int &position, const std::optional<float> &angle)
        : Entity(position, angle) {
    }

    ~ShortLivedEntity() override =default;

    nlohmann::json toJson() final{
        throw std::invalid_argument("Tried to convert a short lived object to JSON : this is illegal");
    }

    bool isJsonSerializable() final {
        return false;
    }

    constexpr std::string getJsonType() final {
        // throw std::invalid_argument("Tried to get the json type of a short lived object : this is illegal"); //can't throw in a constexpr sadly
        return nullptr;
    }
};