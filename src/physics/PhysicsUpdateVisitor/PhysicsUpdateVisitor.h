//
// Created by timon on 11/18/25.
//

#pragma once
#include "PhysicsUpdateAdapter.h"


class PhysicsEntity;
class RoundPhysicsEntity;
class RectPhysicsEntity;

class PhysicsUpdateVisitor : public PhysicsUpdateAdapter
{
public:
    void visitRect(PhysicsEntity * e1, RectPhysicsEntity * e2, SpaceShip * space_ship);
    void visitRound(PhysicsEntity * e1, RoundPhysicsEntity * e2, SpaceShip * space_ship);
    void visitWall(PhysicsEntity * e1, SpaceShip * space_ship);
};


