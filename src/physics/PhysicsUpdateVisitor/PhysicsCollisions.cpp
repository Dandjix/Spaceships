//
// Created by timon on 11/18/25.

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "../shapes/RoundPhysicsShape.h"
#include "debug/CollisionInfo.h"
#include "physics/ConvexSegmentCast.h"
#include "physics/shapes/ConvexPhysicsShape.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/Tile.h"
/**
 * This is applied to two entities that are at the same position to unstuck them
 */
const float jolt = 3;

namespace PhysicsCollisions {
    void applyJolt(PhysicsShape *shape1, PhysicsShape *shape2, SpaceShip *space_ship) {
        float random_angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 360;

        Vector2Float jolt_vector = Vector2Float(jolt, 0.0f).rotate(random_angle);

        shape1->owner_entity->movePosition(jolt_vector, space_ship);
        shape2->owner_entity->movePosition(-jolt_vector, space_ship);
    }

    void visitConvexes(ConvexPhysicsShape *poly1, ConvexPhysicsShape *poly2, SpaceShip *space_ship) {
        float overlap = INFINITY;

        for (int shape = 0; shape < 2; shape++)
        {
            if (shape == 1)
            {
                auto temp = poly1;
                poly1 = poly2;
                poly2 = temp;
            }

            auto poly_1_p = poly1->getVertices();
            auto poly_2_p = poly2->getVertices();

            for (int a = 0; a < poly_1_p.size(); a++)
            {
                int b = (a + 1) % poly_1_p.size();
                Vector2Int axisProj = { -(poly_1_p[b].y - poly_1_p[a].y), poly_1_p[b].x - poly_1_p[a].x };

                // Optional normalisation of projection axis enhances stability slightly
                //float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
                //axisProj = { axisProj.x / d, axisProj.y / d };

                // Work out min and max 1D points for r1
                float min_r1 = INFINITY, max_r1 = -INFINITY;
                for (int p = 0; p < poly_1_p.size(); p++)
                {
                    float q = (poly_1_p[p].x * axisProj.x + poly_1_p[p].y * axisProj.y);
                    min_r1 = std::min(min_r1, q);
                    max_r1 = std::max(max_r1, q);
                }

                // Work out min and max 1D points for r2
                float min_r2 = INFINITY, max_r2 = -INFINITY;
                for (int p = 0; p < poly_2_p.size(); p++)
                {
                    float q = (poly_2_p[p].x * axisProj.x + poly_2_p[p].y * axisProj.y);
                    min_r2 = std::min(min_r2, q);
                    max_r2 = std::max(max_r2, q);
                }

                // Calculate actual overlap along projected axis, and store the minimum
                overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

                if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
                    return;
            }
        }

        // If we got here, the objects have collided, we will displace r1
        // by overlap along the vector between the two object centers
        Vector2Int d = { poly2->getCenter().x - poly1->getCenter().x, poly2->getCenter().y - poly1->getCenter().y };
        float s = sqrtf(d.x*d.x + d.y*d.y);
        Vector2Int new_poly1_center = {poly1->getCenter().x - static_cast<int>(overlap * d.x / s),poly1->getCenter().y - static_cast<int>(overlap * d.y / s)};
        poly1->owner_entity->setPosition(new_poly1_center);
    }

    void visitConvexRound(ConvexPhysicsShape *convex, RoundPhysicsShape *round, SpaceShip *space_ship) {
        if (convex->getCenter() == round->getCenter()) //Jolt if stuck together
            applyJolt(convex, round, space_ship);

        Vector2Int start = round->getCenter();
        auto direction = Vectors::toVector2Float(convex->getCenter() - round->getCenter()).normalized();
        Vector2Int end = round->getCenter() + Vectors::toVector2Int(direction * round->radius);

        Debug::CollisionInfo::instance->addLine(start,end);

        auto intersection = Physics::localSegmentCast(start,end,convex->getVertices());
        if (!intersection.has_value()) return;

        //this is equivalent as if the convex was a circle with a certain radius
        auto convex_radius = (convex->getCenter() - intersection.value()).length();

        //what follows is the same as round on round collision
        Vector2Int diff = convex->getCenter() - round->getCenter();

        float combined_radius = convex_radius + round->radius;

        float force_value = convex_radius + round->radius - diff.length();


        if (force_value <= 0) {
            return;
        }

        float e1_weight = convex->owner_entity->get_weight();
        float e2_weight = round->owner_entity->get_weight();

        float force_e1 = force_value * (e2_weight / (e2_weight + e1_weight));
        float force_e2 = force_value * (e1_weight / (e2_weight + e1_weight));

        auto delta_e1 = (Vectors::toVector2Float(
                             convex->getCenter() - round->getCenter()).normalized() *
                         force_e1);
        auto delta_e2 = (Vectors::toVector2Float(
                             round->getCenter() - convex->getCenter()).normalized() *
                         force_e2);

        convex->owner_entity->movePosition(delta_e1, space_ship);
        round->owner_entity->movePosition(delta_e2, space_ship);

    }

    void visitStaticRoundConvex(RoundStaticPhysicsShape *shape1, ConvexPhysicsShape *shape2, SpaceShip *space_ship) {
    }


    void visitRounds(RoundPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
        // std::cout << "round on round collision" << std::endl;
        if (shape1->getCenter() == shape2->getCenter()) {
            applyJolt(shape1, shape2, space_ship);
        }

        Vector2Int diff = shape1->getCenter() - shape2->getCenter();

        float combined_radius = shape1->radius + shape2->radius;

        float force_value = shape1->radius + shape2->radius - diff.length();


        if (force_value <= 0) {
            return;
        }

        float e1_weight = shape1->owner_entity->get_weight();
        float e2_weight = shape2->owner_entity->get_weight();

        float force_e1 = force_value * (e2_weight / (e2_weight + e1_weight));
        float force_e2 = force_value * (e1_weight / (e2_weight + e1_weight));

        auto delta_e1 = (Vectors::toVector2Float(
                             shape1->getCenter() - shape2->getCenter()).normalized() *
                         force_e1);
        auto delta_e2 = (Vectors::toVector2Float(
                             shape2->getCenter() - shape1->getCenter()).normalized() *
                         force_e2);

        shape1->owner_entity->movePosition(delta_e1, space_ship);
        shape2->owner_entity->movePosition(delta_e2, space_ship);
    }

    void visitStaticRoundRound(RoundStaticPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
        // std::cout << "round on round collision" << std::endl;
        if (shape1->getCenter() == shape2->getCenter()) {
            float random_angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 360;

            Vector2Float jolt_vector = Vector2Float(jolt, 0.0f).rotate(random_angle);

            shape2->owner_entity->movePosition(jolt_vector, space_ship);
        }

        Vector2Int diff = shape1->getCenter() - shape2->getCenter();

        float force_value = shape1->radius + shape2->radius - diff.length();

        if (force_value <= 0) {
            return;
        }

        float force_e2 = force_value;

        auto delta_e2 = (Vectors::toVector2Float(
                             shape2->getCenter() - shape1->getCenter()).normalized() *
                         force_e2);

        shape2->owner_entity->movePosition(delta_e2, space_ship);
    }


    void visitRoundWall(RoundPhysicsShape *shape1, SpaceShip *space_ship) {
        Vector2Int tilesStart;
        Vector2Int tilesEnd;

        auto AABB = shape1->getBoundingBox();

        int factor = Vectors::getFactor();
        int tileWorldSize = factor * Tiles::tileSizePx;

        // Tile range overlapped by the bounding box (+1 margin)
        tilesStart = AABB.topLeft() / tileWorldSize; // - Vector2Int(1, 1);
        tilesEnd = AABB.bottomRight() / tileWorldSize; // + Vector2Int(1, 1);

        Vector2Float center = Vectors::toVector2Float(shape1->getCenter());
        float radius = shape1->radius;

        for (int tx = tilesStart.x; tx <= tilesEnd.x; ++tx) {
            for (int ty = tilesStart.y; ty <= tilesEnd.y; ++ty) {
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

                if (penetration > 0.0f) // Collision!
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


    void visitConvexWall(ConvexPhysicsShape *shape1, SpaceShip *space_ship) {
        // std::cout << "rect on wall collision" << std::endl;
    }
}
