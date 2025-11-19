//
// Created by timon on 11/18/25.
//

#pragma once


class RoundPhysicsEntity;
class RectPhysicsEntity;
class SpaceShip;

class PhysicsUpdateAdapter
{
protected:
    void visitRects(RectPhysicsEntity * e1, RectPhysicsEntity * e2, SpaceShip * space_ship);
    void visitRectRound(RectPhysicsEntity * e1, RoundPhysicsEntity * e2, SpaceShip * space_ship);
    void visitRounds(RoundPhysicsEntity * e1, RoundPhysicsEntity * e2, SpaceShip * space_ship);
    void visitRoundWall(RoundPhysicsEntity * e1, SpaceShip * space_ship);
    void visitRectWall(RectPhysicsEntity * e1, SpaceShip * space_ship);
};
