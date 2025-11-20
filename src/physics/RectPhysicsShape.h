//
// Created by timon on 11/18/25.
//

#pragma once
#include "PhysicsEntity.h"
#include "PhysicsShape.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitorRect.h"


class RectPhysicsShape : public PhysicsShape
{
public:
    Vector2Float dimensions;

    PhysicsUpdateVisitor * createVisitor() override
    {
        return new PhysicsUpdateVisitorRect(this);
    }

    void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship) override
    {
        visitor->visitRect(this,space_ship);
    }

    RectPhysicsShape(PhysicsEntity * owner_entity, Vector2Float dimensions) : PhysicsShape(owner_entity), dimensions(dimensions) {  }
private:
};


