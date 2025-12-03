//
// Created by timon on 12/3/25.
//

#pragma once
#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsUpdateVisitorStaticRound : public PhysicsUpdateVisitor {
protected:
    RoundStaticPhysicsShape * e1;
public:
    explicit PhysicsUpdateVisitorStaticRound(RoundStaticPhysicsShape *e1)
        : e1(e1) {
    }

    ~PhysicsUpdateVisitorStaticRound() override = default;

    void visitRect(RectPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticRoundRect(e1,e2,space_ship);
    }

    void visitRound(RoundPhysicsShape *e2, SpaceShip *space_ship) override {
        return PhysicsCollisions::visitStaticRoundRound(e1,e2,space_ship);
    }

    void visitWall(SpaceShip *space_ship) override {
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override{
    }
};
