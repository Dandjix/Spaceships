//
// Created by timon on 11/18/25.
//

#pragma once


class RoundPhysicsShape;
class RectPhysicsShape;
class SpaceShip;

namespace PhysicsCollisions
{
    void visitRects(RectPhysicsShape * e1, RectPhysicsShape * e2, SpaceShip * space_ship);
    void visitRectRound(RectPhysicsShape * e1, RoundPhysicsShape * e2, SpaceShip * space_ship);
    void visitRounds(RoundPhysicsShape * e1, RoundPhysicsShape * e2, SpaceShip * space_ship);
    void visitRoundWall(RoundPhysicsShape * e1, SpaceShip * space_ship);
    void visitRectWall(RectPhysicsShape * e1, SpaceShip * space_ship);
};
