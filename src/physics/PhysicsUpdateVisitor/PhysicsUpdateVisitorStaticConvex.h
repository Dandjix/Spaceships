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


    void visitConvex(ConvexPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticConvexConvex(shape, e2, space_ship);
    }

    void visitRound(RoundPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticConvexRound(shape, e2, space_ship);
    }


    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override {
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, SpaceShip *space_ship) override {
    }


    void visitWall(SpaceShip *space_ship) override {
    }
};
