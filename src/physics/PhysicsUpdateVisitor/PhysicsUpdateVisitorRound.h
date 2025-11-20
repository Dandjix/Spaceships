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

class PhysicsUpdateVisitorRound : public PhysicsUpdateVisitor
{
protected:
    RoundPhysicsEntity * e1;

public:
    explicit PhysicsUpdateVisitorRound(RoundPhysicsEntity * e1) : PhysicsUpdateVisitor(), e1(e1) {  }

    void visitRect(RectPhysicsEntity* e2, SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRectRound(e2,e1,space_ship);
    }

    void visitRound(RoundPhysicsEntity* e2, SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRounds(e1,e2,space_ship);
    }

    void visitWall(SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitRoundWall(e1,space_ship);
    }
};


