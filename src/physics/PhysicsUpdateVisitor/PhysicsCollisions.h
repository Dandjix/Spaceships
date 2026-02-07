//
// Created by timon on 11/18/25.
//

#pragma once


class RoundPhysicsShape;
class RoundStaticPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

namespace PhysicsCollisions
{
    void visitConvexes(ConvexPhysicsShape * shape1, ConvexPhysicsShape * shape2, SpaceShip * space_ship);
    void visitConvexRound(ConvexPhysicsShape * convex, RoundPhysicsShape * round, SpaceShip * space_ship);
    void visitStaticRoundConvex(RoundStaticPhysicsShape * shape1, ConvexPhysicsShape * shape2, SpaceShip * space_ship);
    void visitRounds(RoundPhysicsShape * shape1, RoundPhysicsShape * shape2, SpaceShip * space_ship);
    void visitStaticRoundRound(RoundStaticPhysicsShape * shape1, RoundPhysicsShape * shape2, SpaceShip * space_ship);
    void visitRoundWall(RoundPhysicsShape * shape1, SpaceShip * space_ship);
    void visitConvexWall(ConvexPhysicsShape * convex, SpaceShip * space_ship);
};
