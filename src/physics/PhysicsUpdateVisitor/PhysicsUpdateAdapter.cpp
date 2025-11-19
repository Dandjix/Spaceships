//
// Created by timon on 11/18/25.
//

#include "PhysicsUpdateAdapter.h"

#include <iostream>

void PhysicsUpdateAdapter::visitRects(RectPhysicsEntity* e1, RectPhysicsEntity* e2, SpaceShip* space_ship)
{
    std::cout << "rect on rect collision" << std::endl;
}

void PhysicsUpdateAdapter::visitRectRound(RectPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
{
    std::cout << "rect on round collision" << std::endl;

}

void PhysicsUpdateAdapter::visitRounds(RoundPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
{
    std::cout << "round on round collision" << std::endl;

}

void PhysicsUpdateAdapter::visitRoundWall(RoundPhysicsEntity* e1, SpaceShip* space_ship)
{
    std::cout << "round on wall collision" << std::endl;

}

void PhysicsUpdateAdapter::visitRectWall(RectPhysicsEntity* e1, SpaceShip* space_ship)
{
    std::cout << "rect on wall collision" << std::endl;
}
