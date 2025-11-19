//
// Created by timon on 11/18/25.
//

#include "PhysicsUpdateVisitor.h"

#include <iostream>
#include "physics/RectPhysicsEntity.h"
#include "physics/RoundPhysicsEntity.h"


class PhysicsEntity;

void PhysicsUpdateVisitor::visitRect(PhysicsEntity* e1, RectPhysicsEntity* e2, SpaceShip* space_ship)
{
    auto rect = dynamic_cast<RectPhysicsEntity*>(e1);
    if (rect != nullptr)
    {
        visitRects(rect,e2,space_ship);
        return;
    }
    auto round = dynamic_cast<RoundPhysicsEntity*>(e1);
    if (round != nullptr)
    {
        visitRectRound(e2,round,space_ship);
        return;
    }
    std::cerr << "A physics entity : " << e1 << " has a type not accounted for by the PhysicsUpdateVisitorRedirect. Add the case to it." << std::endl;
}

void PhysicsUpdateVisitor::visitRound(PhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
{
    auto rect = dynamic_cast<RectPhysicsEntity*>(e1);
    if (rect != nullptr)
    {
        visitRectRound(rect,e2,space_ship);
        return;
    }
    auto round = dynamic_cast<RoundPhysicsEntity*>(e1);
    if (round != nullptr)
    {
        visitRounds(round,e2,space_ship);
        return;
    }
    std::cerr << "A physics entity : " << e1 << " has a type not accounted for by the PhysicsUpdateVisitorRedirect. Add the case to it." << std::endl;
}

void PhysicsUpdateVisitor::visitWall(PhysicsEntity* e1, SpaceShip* space_ship)
{
    auto rect = dynamic_cast<RectPhysicsEntity*>(e1);
    if (rect != nullptr)
    {
        visitRectWall(rect,space_ship);
        return;
    }
    auto round = dynamic_cast<RoundPhysicsEntity*>(e1);
    if (round != nullptr)
    {
        visitRoundWall(round,space_ship);
        return;
    }
    std::cerr << "A physics entity : " << e1 << " has a type not accounted for by the PhysicsUpdateVisitorRedirect. Add the case to it." << std::endl;

}
