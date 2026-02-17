//
// Created by timon on 2/7/26.
//

#pragma once
#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"
#include "physics/shapes/ConvexStaticPhysicsShape.h"

class PhysicsUpdateVisitorStaticConvex : public PhysicsUpdateVisitor {
protected:
    ConvexStaticPhysicsShape *shape;

public:
    explicit PhysicsUpdateVisitorStaticConvex(ConvexStaticPhysicsShape *shape)
        : shape(shape) {
    }


    void visitConvex(ConvexPhysicsShape *e2, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticConvexConvex(shape, e2, instance);
    }

    void visitRound(RoundPhysicsShape *e2, Instances::Instance *instance) override {
        return PhysicsCollisions::visitStaticConvexRound(shape, e2, instance);
    }


    void visitStaticRound(RoundStaticPhysicsShape *e2, Instances::Instance *instance) override {
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, Instances::Instance *instance) override {
    }

    void visitWall(SpaceshipTiles *spaceship_tiles) override {
    }
};
