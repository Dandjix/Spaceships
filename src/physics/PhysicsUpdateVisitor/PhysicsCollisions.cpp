//
// Created by timon on 11/18/25.
//

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "physics/RoundPhysicsEntity.h"

namespace PhysicsCollisions
{
    void visitRects(RectPhysicsEntity* e1, RectPhysicsEntity* e2, SpaceShip* space_ship)
    {
        std::cout << "rect on rect collision" << std::endl;
    }

    void visitRectRound(RectPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
    {
        std::cout << "rect on round collision" << std::endl;
    }

    void visitRounds(RoundPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
    {

    }

    void visitRoundWall(RoundPhysicsEntity* e1, SpaceShip* space_ship)
    {
        std::cout << "round on wall collision" << std::endl;

    }

    void visitRectWall(RectPhysicsEntity* e1, SpaceShip* space_ship)
    {
        std::cout << "rect on wall collision" << std::endl;
    }

}
