//
// Created by timon on 11/18/25.
//

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "physics/RoundPhysicsShape.h"

namespace PhysicsCollisions
{
    void visitRects(RectPhysicsShape* shape1, RectPhysicsShape* shape2, SpaceShip* space_ship)
    {
        // std::cout << "rect on rect collision" << std::endl;
    }

    void visitRectRound(RectPhysicsShape* shape1, RoundPhysicsShape* shape2, SpaceShip* space_ship)
    {
        // std::cout << "rect on round collision" << std::endl;
    }

    void visitRounds(RoundPhysicsShape* shape1, RoundPhysicsShape* shape2, SpaceShip* space_ship)
    {
        // std::cout << "round on round collision" << std::endl;


        Vector2Int diff = shape1->owner_entity->getPosition() - shape2->owner_entity->getPosition();

        float combined_radius = shape1->radius + shape2->radius;

        float force_value = shape1->radius + shape2->radius  - diff.length();


        if (force_value <= 0)
        {
            return;
        }

        float e1_weight = shape1->owner_entity->get_weight();
        float e2_weight = shape2->owner_entity->get_weight();

        float force_e1 = force_value * (e2_weight / (e2_weight + e1_weight));
        float force_e2 = force_value * (e1_weight / (e2_weight + e1_weight));

        auto delta_e1 = (Vectors::toVector2Float(shape1->owner_entity->getPosition()-shape2->owner_entity->getPosition()).normalized() * force_e1);
        auto delta_e2 = (Vectors::toVector2Float(shape2->owner_entity->getPosition()-shape1->owner_entity->getPosition()).normalized() * force_e2);

        shape1->owner_entity->movePosition(delta_e1,space_ship);
        shape2->owner_entity->movePosition(delta_e2,space_ship);

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
