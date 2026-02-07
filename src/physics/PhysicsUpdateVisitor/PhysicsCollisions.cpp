//
// Created by timon on 11/18/25.

#include "PhysicsCollisions.h"

#include <iostream>

#include "math/Vectors.h"
#include "../shapes/RoundPhysicsShape.h"
#include "debug/CollisionInfo.h"
#include "physics/SegmentCast.h"
#include "physics/SegmentCircleCast.h"
#include "physics/shapes/ConvexPhysicsShape.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/Tile.h"
/**
 * This is applied to two entities that are at the same position to unstuck them
 */
constexpr float jolt = 3;

std::vector<std::pair<Vector2Int, Vector2Int> > getDiagonals(Vector2Int center, std::vector<Vector2Int> polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > diagonals;
    diagonals.reserve(polygon.size());

    for (int i = 0; i < polygon.size(); ++i) {
        diagonals.push_back({center, polygon[i]});
    }
    return diagonals;
}

std::vector<std::pair<Vector2Int, Vector2Int> > getSides(std::vector<Vector2Int> polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > sides;
    sides.reserve(polygon.size());

    for (int i = 0; i < polygon.size(); ++i) {
        int next_i = (i + 1) % polygon.size();
        sides.push_back({polygon[i], polygon[next_i]});
    }
    return sides;
}

//https://www.youtube.com/watch?v=7Ik2vowGcU0&t=1320s
bool areCollidingSAT(ConvexPhysicsShape *poly1, ConvexPhysicsShape *poly2) {
    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }

        auto p1 = poly1->getVertices();

        for (auto [edge_start,edge_end]: getSides(poly1->getVertices())) {
            Vector2Float normal = Vector2Float(-edge_end.y + edge_start.y, edge_end.x - edge_start.x);

            float min_r1 = INFINITY, max_r1 = -INFINITY;
            for (auto vertex: poly1->getVertices()) {
                float q = vertex.x * normal.x + vertex.y * normal.y; // dot product
                min_r1 = std::min(min_r1, q);
                max_r1 = std::max(max_r1, q);
            }

            float min_r2 = INFINITY, max_r2 = -INFINITY;
            for (auto vertex: poly2->getVertices()) {
                float q = vertex.x * normal.x + vertex.y * normal.y;
                min_r2 = std::min(min_r2, q);
                max_r2 = std::max(max_r2, q);
            }

            if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
                return false;
        }
    }

    return true;
}


bool areCollidingDiagonals(ConvexPhysicsShape *poly1, ConvexPhysicsShape *poly2) {
    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }
        auto diagonals_1 = getDiagonals(poly1->getCenter(), poly1->getVertices());
        // auto diagonals_2 = getDiagonals(poly2->getCenter(), poly2->getVertices());
        // auto sides_1 = getSides(poly1->getVertices());
        auto sides_2 = getSides(poly2->getVertices());
        for (auto diag: diagonals_1) {
            for (auto side: sides_2) {
                auto res = Physics::segmentIntersectionFloats(diag.first, diag.second, side.first, side.second);
                if (res.success)
                    return true;
            }
        }
    }
    return false;
}

bool circleConvexAreColliding(ConvexPhysicsShape *convex, RoundPhysicsShape *round) {
    //if the convex's center is inside the circle, they are of course colliding.
    if ((convex->getCenter() - round->getCenter()).sqrLength() <= (round->radius * round->radius))
        return true;

    auto sides = getSides(convex->getVertices());

    for (auto [start,end]: sides) {
        if (Physics::segmentCircleIntersectionFloat(start, end, round->getCenter(), round->radius).has_value())
            return true;
    }

    return false;
}

struct PolygonInfo{
    // Vector2Int center;
    std::vector<Vector2Int> vertices;

    PolygonInfo(Vector2Int center, const std::vector<Vector2Int> &vertices) : vertices(vertices) {  }
};

struct SATReturn {
    float overlap;
    bool are_colliding;
};
SATReturn SeparatedAxisTheorem(PolygonInfo * poly1, PolygonInfo * poly2) {
    float overlap = INFINITY;

    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }

        auto p1 = poly1->vertices;

        for (auto [edge_start,edge_end]: getSides(poly1->vertices)) {
            Vector2Float normal = Vector2Float(-edge_end.y + edge_start.y, edge_end.x - edge_start.x).normalized();

            float min_r1 = INFINITY, max_r1 = -INFINITY;
            for (auto vertex: poly1->vertices) {
                float q = vertex.x * normal.x + vertex.y * normal.y; // dot product
                min_r1 = std::min(min_r1, q);
                max_r1 = std::max(max_r1, q);
            }

            float min_r2 = INFINITY, max_r2 = -INFINITY;
            for (auto vertex: poly2->vertices) {
                float q = vertex.x * normal.x + vertex.y * normal.y;
                min_r2 = std::min(min_r2, q);
                max_r2 = std::max(max_r2, q);
            }

            overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

            if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
                return {0,false};
        }
    }

    return {overlap,true};
}


// Vector2Float direction(Vector2Int start, Vector2Int end) {
//     return Vectors::toVector2Float(end - start).normalized();
// }

namespace PhysicsCollisions {
    void applyJolt(PhysicsShape *shape1, PhysicsShape *shape2, SpaceShip *space_ship) {
        float random_angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 360;

        Vector2Float jolt_vector = Vector2Float(jolt, 0.0f).rotate(random_angle);

        shape1->owner_entity->movePosition(jolt_vector, space_ship);
        shape2->owner_entity->movePosition(-jolt_vector, space_ship);
    }


    void visitConvexes(ConvexPhysicsShape *poly1, ConvexPhysicsShape *poly2, SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo poly1_info = {poly1->getCenter(),poly1->getVertices()};
            PolygonInfo poly2_info = {poly2->getCenter(),poly2->getVertices()};
            res = SeparatedAxisTheorem(&poly1_info,&poly2_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(poly1->getCenter() - poly2->getCenter()).normalized() * res.overlap;

        float e1_weight = poly1->owner_entity->get_weight();
        float e2_weight = poly2->owner_entity->get_weight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        poly1->owner_entity->movePosition(delta * force_e1,space_ship);
        poly2->owner_entity->movePosition(-delta * force_e2,space_ship);
    }

    // here's a little lesson in trickery, this is going down in history
    void visitConvexRound(ConvexPhysicsShape *convex, RoundPhysicsShape *round, SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo convex_info = {convex->getCenter(),convex->getVertices()};
            PolygonInfo round_info = {round->getCenter(),round->generateVertices()};
            res = SeparatedAxisTheorem(&convex_info,&round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(convex->getCenter() - round->getCenter()).normalized() * res.overlap;

        float e1_weight = convex->owner_entity->get_weight();
        float e2_weight = round->owner_entity->get_weight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        convex->owner_entity->movePosition(delta * force_e1,space_ship);
        round->owner_entity->movePosition(-delta * force_e2,space_ship);
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
