//
// Created by timon on 11/18/25.
//

#pragma once
#include "PhysicsEntity.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitorRect.h"


class RectPhysicsEntity : public PhysicsEntity
{
public:
    RectPhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, Behavior* behavior)
        : PhysicsEntity(position, angle, behavior)
    {
    }

    PhysicsUpdateVisitor * createVisitor() override
    {
        return new PhysicsUpdateVisitorRect(this);
    }

    void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship) override
    {
        visitor->visitRect(this,space_ship);
    }

private:
};


