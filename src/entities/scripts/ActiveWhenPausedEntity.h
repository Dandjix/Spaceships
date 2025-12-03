//
// Created by timon on 12/3/25.
//

#pragma once
#include "Entity.h"
#include "ShortLivedEntity.h"


class ActiveWhenPausedEntity : public ShortLivedEntity{
public:
    ActiveWhenPausedEntity(const Vector2Int &position, const std::optional<float> &angle)
        : ShortLivedEntity(position, angle) {
    }

    ~ActiveWhenPausedEntity() override = default;

    void registerInSpaceship(SpaceShip *space_ship) override;

    void unregisterInSpacehip(SpaceShip *space_ship) override;
};

