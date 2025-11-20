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
        // std::cout << "rect on rect collision" << std::endl;
    }

    void visitRectRound(RectPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
    {
        // std::cout << "rect on round collision" << std::endl;
    }

    void visitRounds(RoundPhysicsEntity* e1, RoundPhysicsEntity* e2, SpaceShip* space_ship)
    {
        Vector2Int diff = e1->getPosition() - e2->getPosition();

        float combined_radius = e1->getRadius() + e2->getRadius();

        // if ( diff.sqrLength()>=  static_cast<int>(combined_radius*combined_radius))
        // {
        //     std::cout << "exiting : diff greater than radius" << std::endl;
        //     return;
        // }

        float force_value = Scaling::scaleToWorld(e1->getRadius()) + Scaling::scaleToWorld(e2->getRadius())  - diff.length();

        // std::cout << "diff length : " << diff.length() << std::endl;

        if (force_value <= 0)
        {
            // std::cout << "exiting : force_value 0" << std::endl;
            return;
        }

        // std::cout << "force of " << force_value << "applied" << std::endl;

        float force_e1 = force_value * (e2->get_weight()/e1->get_weight());
        float force_e2 = force_value * (e1->get_weight()/e2->get_weight());

        auto delta_e1 = (Vectors::toVector2Float(e1->getPosition()-e2->getPosition()) * force_e1);
        auto delta_e2 = (Vectors::toVector2Float(e2->getPosition()-e1->getPosition()) * force_e2);

        e1->movePosition(delta_e1,space_ship);
        e1->movePosition(delta_e2,space_ship);

    }


    void visitRoundWall(RoundPhysicsEntity* e1, SpaceShip* space_ship)
    {
        // std::cout << "round on wall collision" << std::endl;

    }

    void visitRectWall(RectPhysicsEntity* e1, SpaceShip* space_ship)
    {
        // std::cout << "rect on wall collision" << std::endl;
    }

}
