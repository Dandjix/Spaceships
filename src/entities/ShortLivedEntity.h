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

    nlohmann::json toJson() override {
        throw std::invalid_argument("Tried to convert a short lived object to JSON : this is illegal");
    }

    bool isJsonSerializable() override {
        return false;
    }
};