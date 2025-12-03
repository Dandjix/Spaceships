//
// Created by timon on 12/3/25.
//

#pragma once
#include "Entity.h"
#include "math/Vectors.h"




class LateUpdateEntity : public Entity{
public:
    LateUpdateEntity(const Vector2Int &position, const std::optional<float> &angle);

    ~LateUpdateEntity() override = default;

    void registerInSpaceship(SpaceShip *space_ship) override;

    void unregisterInSpacehip(SpaceShip *space_ship) override;

    virtual void lateUpdate(const UpdateContext & context) = 0;
};
