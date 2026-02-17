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

    void visitConvex(ConvexPhysicsShape* e2, Instances::Instance *instance) override
    {
        PhysicsCollisions::visitConvexes(e1,e2,instance);
    }

    void visitRound(RoundPhysicsShape* e2, Instances::Instance *instance) override
    {
        PhysicsCollisions::visitConvexRound(e1,e2,instance);
    }

    void visitWall(SpaceshipTiles *spaceship_tiles) override
    {
        PhysicsCollisions::visitConvexWall(e1,spaceship_tiles);
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticRoundConvex(e2,e1,instance);
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticConvexConvex(static_convex,e1,instance);
    }
};
