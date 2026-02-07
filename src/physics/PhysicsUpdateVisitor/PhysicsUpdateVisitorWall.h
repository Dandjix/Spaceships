//
// Created by timon on 11/19/25.
//

#pragma once
#include <stdexcept>
#include <SDL3/SDL_log.h>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

class PhysicsUpdateVisitorWall : public PhysicsUpdateVisitor {
public:
    PhysicsUpdateVisitorWall() : PhysicsUpdateVisitor() {
    }

    void visitConvex(ConvexPhysicsShape *e2, SpaceShip *space_ship) override {
        PhysicsCollisions::visitConvexWall(e2, space_ship);
    }

    void visitRound(RoundPhysicsShape *e2, SpaceShip *space_ship) override {
        PhysicsCollisions::visitRoundWall(e2, space_ship);
    }

    void visitWall(SpaceShip *space_ship) override {
        throw std::invalid_argument(
            "Uhhhh you tried to make a wall collide with a wall... Gone a little bonjour have you now ?");
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, SpaceShip *space_ship) override {
        //This is left empty for there is no collision between walls and static
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, SpaceShip *space_ship) override {
    }
};
