//
// Created by timon on 11/19/25.
//

#pragma once
#include <optional>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class RoundStaticPhysicsShape;
class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

class PhysicsUpdateVisitorRound : public PhysicsUpdateVisitor {
protected:
    RoundPhysicsShape *e1;

public:
    explicit PhysicsUpdateVisitorRound(RoundPhysicsShape *e1) : PhysicsUpdateVisitor(), e1(e1) {
    }

    void visitConvex(ConvexPhysicsShape *e2, Instances::Instance *instance) override {
        PhysicsCollisions::visitConvexRound(e2, e1, instance);
    }

    void visitRound(RoundPhysicsShape *e2, Instances::Instance *instance) override {
        PhysicsCollisions::visitRounds(e1, e2, instance);
    }

    void visitWall(SpaceshipTiles *spaceship_tiles) override {
        PhysicsCollisions::visitRoundWall(e1, spaceship_tiles);
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, Instances::Instance *instance) override {
        PhysicsCollisions::visitStaticRoundRound(e2, e1, instance);
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, Instances::Instance *instance) override {
        PhysicsCollisions::visitStaticConvexRound(static_convex,e1,instance);
    }
};
