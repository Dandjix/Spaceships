//
// Created by timon on 11/18/25.
//

#pragma once


class PhysicsEntity;
class RoundPhysicsEntity;
class RectPhysicsEntity;
class SpaceShip;

class PhysicsUpdateVisitor
{
public:
    explicit PhysicsUpdateVisitor()= default;
    virtual ~PhysicsUpdateVisitor() = default;

    virtual void visitRect(RectPhysicsEntity * e2, SpaceShip* space_ship)=0;
    virtual void visitRound(RoundPhysicsEntity * e2, SpaceShip* space_ship)=0;
    virtual void visitWall(SpaceShip* space_ship)=0;
};


