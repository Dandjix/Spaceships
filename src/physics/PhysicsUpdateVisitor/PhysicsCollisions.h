//
// Created by timon on 11/18/25.
//

#pragma once
#include "spaceships/SpaceShip.h"


class ConvexStaticPhysicsShape;
class PhysicsUpdateVisitorStaticConvex;
class RoundPhysicsShape;
class RoundStaticPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;

namespace PhysicsCollisions
{
    void visitConvexes(ConvexPhysicsShape * shape1, ConvexPhysicsShape * shape2, Instances::Instance * instance);
    void visitConvexRound(ConvexPhysicsShape * convex, RoundPhysicsShape * round, Instances::Instance * instance);
    void visitStaticRoundConvex(RoundStaticPhysicsShape * shape1, ConvexPhysicsShape * shape2, Instances::Instance * instance);
    void visitRounds(RoundPhysicsShape * shape1, RoundPhysicsShape * shape2, Instances::Instance * instance);

    void visitStaticConvexConvex(ConvexStaticPhysicsShape * static_convex, ConvexPhysicsShape * convex, Instances::Instance * instance);
    void visitStaticConvexRound(ConvexStaticPhysicsShape * static_convex, RoundPhysicsShape * round, Instances::Instance * instance);
    void visitStaticRoundRound(RoundStaticPhysicsShape * shape1, RoundPhysicsShape * shape2, Instances::Instance * instance);

    void visitRoundWall(RoundPhysicsShape * shape1, SpaceshipTiles *tiles);
    void visitConvexWall(ConvexPhysicsShape * convex, SpaceshipTiles *tiles);
};
