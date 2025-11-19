//
// Created by timon on 11/18/25.
//

#pragma once
#include "PhysicsEntity.h"


class RectPhysicsEntity : public PhysicsEntity
{
public:
    RectPhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, Behavior* behavior)
        : PhysicsEntity(position, angle, behavior)
    {
    }

private:
    void beVisitedBy(PhysicsEntity* other, PhysicsUpdateVisitor* visitor, SpaceShip * space_ship) override;
};


