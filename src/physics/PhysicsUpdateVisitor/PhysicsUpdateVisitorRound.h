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

    void visitConvex(ConvexPhysicsShape *e2, SpaceShip *space_ship) override {
        PhysicsCollisions::visitConvexRound(e2, e1, space_ship);
    }

    void visitRound(RoundPhysicsShape *e2, SpaceShip *space_ship) override {
        PhysicsCollisions::visitRounds(e1, e2, space_ship);
    }

    void visitWall(SpaceShip *space_ship) override {
        PhysicsCollisions::visitRoundWall(e1, space_ship);
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override {
        PhysicsCollisions::visitStaticRoundRound(e2, e1, space_ship);
    }
};
