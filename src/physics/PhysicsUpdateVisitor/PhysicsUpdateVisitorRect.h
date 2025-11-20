//
// Created by timon on 11/19/25.
//

#pragma once
#include <optional>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsEntity;
class RoundPhysicsEntity;
class RectPhysicsEntity;
class SpaceShip;

class PhysicsUpdateVisitorRect : public PhysicsUpdateVisitor
{
protected:
    RectPhysicsEntity * e1;

public:
    explicit PhysicsUpdateVisitorRect(RectPhysicsEntity * e1) : PhysicsUpdateVisitor(), e1(e1) {  }

    void visitRect(RectPhysicsEntity* e2, SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRects(e1,e2,space_ship);
    }

    void visitRound(RoundPhysicsEntity* e2, SpaceShip * space_ship) override
    {
        PhysicsCollisions::visitRectRound(e1,e2,space_ship);
    }

    void visitWall(SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRectWall(e1,space_ship);
    }
};
