//
// Created by timon on 11/18/25.
//

#pragma once


class RoundPhysicsShape;
class RoundStaticPhysicsShape;
class RectPhysicsShape;
class SpaceShip;

namespace PhysicsCollisions
{
    void visitRects(RectPhysicsShape * shape1, RectPhysicsShape * shape2, SpaceShip * space_ship);
    void visitRectRound(RectPhysicsShape * shape1, RoundPhysicsShape * shape2, SpaceShip * space_ship);
    void visitStaticRoundRect(RoundStaticPhysicsShape * shape1, RectPhysicsShape * shape2, SpaceShip * space_ship);
    void visitRounds(RoundPhysicsShape * shape1, RoundPhysicsShape * shape2, SpaceShip * space_ship);
    void visitStaticRoundRound(RoundStaticPhysicsShape * shape1, RoundPhysicsShape * shape2, SpaceShip * space_ship);
    void visitRoundWall(RoundPhysicsShape * shape1, SpaceShip * space_ship);
    void visitRectWall(RectPhysicsShape * shape1, SpaceShip * space_ship);
};
