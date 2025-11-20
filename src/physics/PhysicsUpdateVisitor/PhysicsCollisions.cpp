//
// Created by timon on 11/18/25.
//

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "physics/RoundPhysicsShape.h"

namespace PhysicsCollisions
{
    void visitRects(RectPhysicsShape* e1, RectPhysicsShape* e2, SpaceShip* space_ship)
    {
        // std::cout << "rect on rect collision" << std::endl;
    }

    void visitRectRound(RectPhysicsShape* e1, RoundPhysicsShape* e2, SpaceShip* space_ship)
    {
        // std::cout << "rect on round collision" << std::endl;
    }

    void visitRounds(RoundPhysicsShape* e1, RoundPhysicsShape* e2, SpaceShip* space_ship)
    {
        Vector2Int diff = e1->owner_entity->getPosition() - e2->owner_entity->getPosition();

        float combined_radius = e1->radius + e2->radius;

        // if ( diff.sqrLength()>=  static_cast<int>(combined_radius*combined_radius))
        // {
        //     std::cout << "exiting : diff greater than radius" << std::endl;
        //     return;
        // }

        float force_value = Scaling::scaleToWorld(e1->radius) + Scaling::scaleToWorld(e2->radius)  - diff.length();

        // std::cout << "diff length : " << diff.length() << std::endl;

        if (force_value <= 0)
        {
            // std::cout << "exiting : force_value 0" << std::endl;
            return;
        }

        // std::cout << "force of " << force_value << "applied" << std::endl;

        float force_e1 = force_value * (e2->get_weight()/e1->get_weight());
        float force_e2 = force_value * (e1->get_weight()/e2->get_weight());

        auto delta_e1 = (Vectors::toVector2Float(e1->owner_entity->getPosition()-e2->owner_entity->getPosition()) * force_e1);
        auto delta_e2 = (Vectors::toVector2Float(e2->owner_entity->getPosition()-e1->owner_entity->getPosition()) * force_e2);

        e1->owner_entity->movePosition(delta_e1,space_ship);
        e1->owner_entity->movePosition(delta_e2,space_ship);

    }


    void visitRoundWall(RoundPhysicsShape* e1, SpaceShip* space_ship)
    {
        // std::cout << "round on wall collision" << std::endl;

    }

    void visitRectWall(RectPhysicsShape* e1, SpaceShip* space_ship)
    {
        // std::cout << "rect on wall collision" << std::endl;
    }

}
