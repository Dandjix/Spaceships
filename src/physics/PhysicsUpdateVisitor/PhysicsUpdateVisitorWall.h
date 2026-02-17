//
// Created by timon on 11/19/25.
//

#pragma once
#include <stdexcept>

#include "PhysicsCollisions.h"
#include "PhysicsUpdateVisitor.h"

class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

class PhysicsUpdateVisitorWall : public PhysicsUpdateVisitor {
protected:
    SpaceshipTiles * spaceship_tiles;
public:
    explicit PhysicsUpdateVisitorWall(SpaceshipTiles * spaceship_tiles) : PhysicsUpdateVisitor(), spaceship_tiles(spaceship_tiles) {
    }

    void visitConvex(ConvexPhysicsShape *e2, Instances::Instance *instance) override {
        PhysicsCollisions::visitConvexWall(e2, spaceship_tiles);
    }

    void visitRound(RoundPhysicsShape *e2, Instances::Instance *instance) override {
        PhysicsCollisions::visitRoundWall(e2, spaceship_tiles);
    }

    void visitWall(SpaceshipTiles *_) override {
        throw std::invalid_argument(
            "Uhhhh you tried to make a wall collide with a wall... Gone a little bonjour have you now ?");
    }

    void visitStaticRound(RoundStaticPhysicsShape *e2, Instances::Instance *instance) override {
        //This is left empty for there is no collision between walls and static
    }

    void visitStaticConvex(ConvexStaticPhysicsShape *static_convex, Instances::Instance *instance) override {
    }
};
