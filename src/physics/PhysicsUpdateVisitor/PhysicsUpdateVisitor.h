//
// Created by timon on 11/18/25.
//

#pragma once


class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class SpaceShip;
class RoundStaticPhysicsShape;

class PhysicsUpdateVisitor
{
public:
    explicit PhysicsUpdateVisitor()= default;
    virtual ~PhysicsUpdateVisitor() = default;

    virtual void visitConvex(ConvexPhysicsShape * e2, SpaceShip* space_ship)=0;
    virtual void visitRound(RoundPhysicsShape * e2, SpaceShip* space_ship)=0;
    virtual void visitStaticRound(RoundStaticPhysicsShape * e2, SpaceShip* space_ship)=0;
    virtual void visitWall(SpaceShip* space_ship)=0;
};


