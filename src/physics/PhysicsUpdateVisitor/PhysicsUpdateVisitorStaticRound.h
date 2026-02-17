//
// Created by timon on 12/3/25.
//

#pragma once
#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsUpdateVisitorStaticRound : public PhysicsUpdateVisitor {
protected:
    RoundStaticPhysicsShape *e1;

public:
    explicit PhysicsUpdateVisitorStaticRound(RoundStaticPhysicsShape *e1)
        : e1(e1) {
    }

    ~PhysicsUpdateVisitorStaticRound() override = default;

    void visitConvex(ConvexPhysicsShape *e2, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticRoundConvex(e1, e2, instance);
    }

    void visitRound(RoundPhysicsShape *e2, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticRoundRound(e1, e2, instance);
    }

    void visitWall(SpaceshipTiles *spaceship_tiles) override {
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, Instances::Instance *instance) override {
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, Instances::Instance *instance) override {
    }
};
