//
// Created by timon on 11/19/25.
//

#pragma once
#include <optional>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsEntity;
class RoundPhysicsShape;
class RectPhysicsShape;
class SpaceShip;

class PhysicsUpdateVisitorRect : public PhysicsUpdateVisitor
{
protected:
    RectPhysicsShape * e1;

public:
    explicit PhysicsUpdateVisitorRect(RectPhysicsShape * e1) : PhysicsUpdateVisitor(), e1(e1) {  }

    void visitRect(RectPhysicsShape* e2, SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRects(e1,e2,space_ship);
    }

    void visitRound(RoundPhysicsShape* e2, SpaceShip * space_ship) override
    {
        PhysicsCollisions::visitRectRound(e1,e2,space_ship);
    }

    void visitWall(SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRectWall(e1,space_ship);
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticRoundRect(e2,e1,space_ship);
    }
};
