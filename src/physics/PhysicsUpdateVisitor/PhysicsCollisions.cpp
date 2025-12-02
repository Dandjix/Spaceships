//
// Created by timon on 11/18/25.

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "physics/RoundPhysicsShape.h"

/**
 * This is applied to two entities that are at the same position to unstuck them
 */
const float jolt = 3;

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

    void applyJolt(RoundPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
        float random_angle = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*360;

        Vector2Float jolt_vector = Vector2Float(jolt,0.0f).rotate(random_angle);

        shape1->owner_entity->movePosition(jolt_vector,space_ship);
        shape2->owner_entity->movePosition(-jolt_vector,space_ship);
    }

    void visitRounds(RoundPhysicsShape* shape1, RoundPhysicsShape* shape2, SpaceShip* space_ship)
    {
        // std::cout << "round on round collision" << std::endl;
        if (shape1->owner_entity->getPosition() == shape2->owner_entity->getPosition()) {

            applyJolt(shape1, shape2, space_ship);
        }

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


void visitRoundWall(RoundPhysicsShape* shape1, SpaceShip* space_ship)
{
    Vector2Int tilesStart;
    Vector2Int tilesEnd;

    auto AABB = shape1->getBoundingBox();

    int factor = Vectors::getFactor();
    int tileWorldSize = factor * Tiles::tileSizePx;

    // Tile range overlapped by the bounding box (+1 margin)
    tilesStart = AABB.topLeft() / tileWorldSize;// - Vector2Int(1, 1);
    tilesEnd   = AABB.bottomRight() / tileWorldSize;// + Vector2Int(1, 1);

    Vector2Float center = Vectors::toVector2Float(shape1->owner_entity->getPosition());
    float radius = shape1->radius;

    for (int tx = tilesStart.x; tx <= tilesEnd.x; ++tx)
    {
        for (int ty = tilesStart.y; ty <= tilesEnd.y; ++ty)
        {
            Tile tile = space_ship->getSpaceshipTiles().get_tile(tx, ty);

            // Only collide with solid (or wall) tiles
            if (tile != Tile::Wall)
                continue;

            // Compute the tile's AABB in world coordinates
            Vector2Float tileMin(tx * tileWorldSize,
                                 ty * tileWorldSize);

            Vector2Float tileMax = tileMin + Vector2Float(tileWorldSize, tileWorldSize);

            // Closest point on tile to the circle center
            Vector2Float closest;
            closest.x = std::max(tileMin.x, std::min(center.x, tileMax.x));
            closest.y = std::max(tileMin.y, std::min(center.y, tileMax.y));

            Vector2Float diff = center - closest;
            float dist = diff.length();

            float penetration = radius - dist;

            if (penetration > 0.0f)  // Collision!
            {
                // If center is EXACTLY inside the tile center (rare), avoid NaN
                Vector2Float normal;
                if (dist == 0)
                    normal = Vector2Float(0, -1);
                else
                    normal = diff.normalized();

                Vector2Float delta = normal * penetration;

                // Apply collision correction
                shape1->owner_entity->movePosition(delta, space_ship);

                // Update center because we moved
                center = center + delta;
            }
        }
    }
}


    void visitRectWall(RectPhysicsShape* shape1, SpaceShip* space_ship)
    {
        // std::cout << "rect on wall collision" << std::endl;
    }

}
