//
// Created by timon on 11/19/25.
//

#pragma once
#include <optional>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

class PhysicsUpdateVisitorConvex : public PhysicsUpdateVisitor
{
protected:
    ConvexPhysicsShape * e1;

public:
    explicit PhysicsUpdateVisitorConvex(ConvexPhysicsShape * e1) : PhysicsUpdateVisitor(), e1(e1) {  }

    void visitConvex(ConvexPhysicsShape* e2, SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitConvexes(e1,e2,space_ship);
    }

    void visitRound(RoundPhysicsShape* e2, SpaceShip * space_ship) override
    {
        PhysicsCollisions::visitConvexRound(e1,e2,space_ship);
    }

    void visitWall(SpaceShip* space_ship) override
    {
        PhysicsCollisions::visitConvexWall(e1,space_ship);
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticRoundConvex(e2,e1,space_ship);
    }
};
