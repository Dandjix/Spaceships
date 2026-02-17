//
// Created by timon on 11/18/25.
//

#pragma once


class PhysicsEntity;
class RoundPhysicsShape;
class ConvexPhysicsShape;
class ConvexStaticPhysicsShape;
class SpaceShip;
class RoundStaticPhysicsShape;

class PhysicsUpdateVisitor
{
public:
    explicit PhysicsUpdateVisitor()= default;
    virtual ~PhysicsUpdateVisitor() = default;

    virtual void visitConvex(ConvexPhysicsShape * e2, Instances::Instance *instance)=0;
    virtual void visitRound(RoundPhysicsShape * e2, Instances::Instance *instance)=0;
    virtual void visitStaticConvex(ConvexStaticPhysicsShape * static_convex, Instances::Instance *instance)=0;
    virtual void visitStaticRound(RoundStaticPhysicsShape * e2, Instances::Instance *instance)=0;
    virtual void visitWall(SpaceshipTiles *spaceship_tiles)=0;

};


