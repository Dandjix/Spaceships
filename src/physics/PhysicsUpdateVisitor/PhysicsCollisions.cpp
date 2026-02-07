//
// Created by timon on 11/18/25.

#include "PhysicsCollisions.h"


#include "math/Vectors.h"
#include "../shapes/RoundPhysicsShape.h"
#include "physics/Physics.h"
#include "physics/SegmentCast.h"
#include "physics/SegmentCircleCast.h"
#include "physics/shapes/ConvexPhysicsShape.h"
#include "physics/shapes/ConvexStaticPhysicsShape.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/Tile.h"
/**
 * This is applied to two entities that are at the same position to unstuck them
 */
constexpr float jolt = 3;

std::vector<std::pair<Vector2Int, Vector2Int> > getDiagonals(Vector2Int center, const std::vector<Vector2Int>& polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > diagonals;
    diagonals.reserve(polygon.size());

    for (auto & i : polygon) {
        diagonals.emplace_back(center, i);
    }
    return diagonals;
}

std::vector<std::pair<Vector2Int, Vector2Int> > getSides(std::vector<Vector2Int> polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > sides;
    sides.reserve(polygon.size());

    for (int i = 0; i < polygon.size(); ++i) {
        std::size_t next_i = (i + 1) % polygon.size();
        sides.emplace_back(polygon[i], polygon[next_i]);
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
            Vector2Float normal = Vector2Float(static_cast<float>(-edge_end.y + edge_start.y), static_cast<float>(edge_end.x - edge_start.x));

            float min_r1 = INFINITY, max_r1 = -INFINITY;
            for (auto vertex: poly1->getVertices()) {
                float q = static_cast<float>(vertex.x) * normal.x + static_cast<float>(vertex.y) * normal.y; // dot product
                min_r1 = std::min(min_r1, q);
                max_r1 = std::max(max_r1, q);
            }

            float min_r2 = INFINITY, max_r2 = -INFINITY;
            for (auto vertex: poly2->getVertices()) {
                float q = static_cast<float>(vertex.x) * normal.x + static_cast<float>(vertex.y) * normal.y;
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
            for (auto [start, end]: sides_2) {
                auto res = Physics::segmentIntersectionFloats(diag.first, diag.second, start, end);
                if (res.success)
                    return true;
            }
        }
    }
    return false;
}

bool circleConvexAreColliding(ConvexPhysicsShape *convex, RoundPhysicsShape *round) {
    //if the convex's center is inside the circle, they are of course colliding.
    if ((convex->getCenter() - round->getCenter()).sqrLength() <= static_cast<float>(round->radius * round->radius))
        return true;

    auto sides = getSides(convex->getVertices());

    return std::ranges::any_of(sides, [&](const auto& side) {
        auto [start, end] = side;
        return Physics::segmentCircleIntersectionFloat(start, end, round->getCenter(), round->radius).has_value();
    });
}

struct PolygonInfo {
    // Vector2Int center;
    std::vector<Vector2Int> vertices;

    PolygonInfo(Vector2Int center, const std::vector<Vector2Int> &vertices) : vertices(vertices) {
    }
};

struct SATReturn {
    float overlap;
    bool are_colliding;
};

SATReturn SeparatedAxisTheorem(PolygonInfo *poly1, PolygonInfo *poly2) {
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
                return {0, false};
        }
    }

    return {overlap, true};
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
            PolygonInfo poly1_info = {poly1->getCenter(), poly1->getVertices()};
            PolygonInfo poly2_info = {poly2->getCenter(), poly2->getVertices()};
            res = SeparatedAxisTheorem(&poly1_info, &poly2_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(poly1->getCenter() - poly2->getCenter()).normalized() * res.
                             overlap;

        float e1_weight = poly1->owner_entity->get_weight();
        float e2_weight = poly2->owner_entity->get_weight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        poly1->owner_entity->movePosition(delta * force_e1, space_ship);
        poly2->owner_entity->movePosition(-delta * force_e2, space_ship);
    }

    // here's a little lesson in trickery, this is going down in history
    void visitConvexRound(ConvexPhysicsShape *convex, RoundPhysicsShape *round, SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo convex_info = {convex->getCenter(), convex->getVertices()};
            PolygonInfo round_info = {round->getCenter(), round->generateVertices()};
            res = SeparatedAxisTheorem(&convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(convex->getCenter() - round->getCenter()).normalized() * res.
                             overlap;

        float e1_weight = convex->owner_entity->get_weight();
        float e2_weight = round->owner_entity->get_weight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        convex->owner_entity->movePosition(delta * force_e1, space_ship);
        round->owner_entity->movePosition(-delta * force_e2, space_ship);
    }

    void visitStaticRoundConvex(RoundStaticPhysicsShape *static_round, ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo convex_info = {convex->getCenter(), convex->getVertices()};
            PolygonInfo round_info = {static_round->getCenter(), static_round->generateVertices()};
            res = SeparatedAxisTheorem(&convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(convex->getCenter() - static_round->getCenter()).normalized() * res.
                             overlap;

        convex->owner_entity->movePosition(delta, space_ship);
    }


    void visitRounds(RoundPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
        if (shape1->getCenter() == shape2->getCenter()) {
            applyJolt(shape1, shape2, space_ship);
        }

        Vector2Int diff = shape1->getCenter() - shape2->getCenter();

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

    void visitStaticConvexConvex(ConvexStaticPhysicsShape *static_convex, ConvexPhysicsShape *convex,
        SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo static_convex_info = {static_convex->getCenter(), static_convex->getVertices()};
            PolygonInfo convex_info = {convex->getCenter(), convex->getVertices()};
            res = SeparatedAxisTheorem(&static_convex_info, &convex_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(static_convex->getCenter() - convex->getCenter()).normalized() * res.
                             overlap;

        convex->owner_entity->movePosition(-delta, space_ship);
    }

    void visitStaticConvexRound(ConvexStaticPhysicsShape *static_convex, RoundPhysicsShape *round,
        SpaceShip *space_ship) {
        SATReturn res;
        {
            PolygonInfo static_convex_info = {static_convex->getCenter(), static_convex->getVertices()};
            PolygonInfo round_info = {round->getCenter(), round->generateVertices()};
            res = SeparatedAxisTheorem(&static_convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(static_convex->getCenter() - round->getCenter()).normalized() * res.
                             overlap;

        round->owner_entity->movePosition(-delta, space_ship);
    }

    void visitStaticRoundRound(RoundStaticPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
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
        auto AABB = shape1->getBoundingBox();

        int tileWorldSize = Scaling::scaleToWorld(Tiles::tileSizePx);

        Vector2Float center = Vectors::toVector2Float(shape1->getCenter());
        float radius = shape1->radius;

        // Tile range overlapped by the bounding box (+1 margin)
        Vector2Int tilesStart = AABB.topLeft() / tileWorldSize; // - Vector2Int(1, 1);
        Vector2Int tilesEnd = AABB.bottomRight() / tileWorldSize; // + Vector2Int(1, 1);
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

    struct HitInfo {
        Vector2Int hit;
        float original_distance;
    };

    void diagonalsDisplacement(ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        std::vector<HitInfo> hits;

        for (auto [start,end]: getDiagonals(convex->getCenter(), convex->getVertices())) {
            auto diff = end - start;
            float original_distance = diff.length();
            auto res = Physics::RayCast(start, Vectors::toVector2Float(diff), space_ship, original_distance);
            if (!res.hit)
                continue;
            hits.push_back({res.hit_world_position, original_distance});
        }

        Vector2Float displacement = {0, 0};
        int hit_count = 0;
        for (auto hit: hits) {
            auto diff = hit.hit - convex->getCenter();
            float hit_distance_to_center = diff.length();

            Vector2Float direction = Vectors::toVector2Float(diff);

            // Calculate penetration: how far the vertex is past the wall
            float penetration = hit.original_distance - hit_distance_to_center;

            if (penetration > 0) {
                // Push away from the wall along the ray direction
                displacement += -direction.normalized() * penetration;
                hit_count++;
            }
        }

        // Average the displacement if multiple vertices hit.
        if (hit_count > 0) {
            convex->owner_entity->movePosition(displacement / hit_count, space_ship);
        }
    }

    void iterateCornerNormals(const BoundingBox<int> &AABB, SpaceShip *space_ship,
                              const std::function<void(Vector2Int world_position, Vector2Float normal)> &func) {
        int tileWorldSize = Scaling::scaleToWorld(Tiles::tileSizePx);
        Vector2Int cornersStart = AABB.topLeft() / tileWorldSize;
        Vector2Int cornersEnd = AABB.bottomRight() / tileWorldSize + Vector2Int(1, 1);

        for (int corner_x = cornersStart.x; corner_x <= cornersEnd.x; ++corner_x) {
            for (int corner_y = cornersStart.y; corner_y <= cornersEnd.y; ++corner_y) {
                // Check the 4 tiles that share this corner
                // Top-left, top-right, bottom-left, bottom-right relative to corner
                bool tl = space_ship->getSpaceshipTiles().get_tile(corner_x - 1, corner_y - 1) == Tile::Wall;
                bool tr = space_ship->getSpaceshipTiles().get_tile(corner_x, corner_y - 1) == Tile::Wall;
                bool bl = space_ship->getSpaceshipTiles().get_tile(corner_x - 1, corner_y) == Tile::Wall;
                bool br = space_ship->getSpaceshipTiles().get_tile(corner_x, corner_y) == Tile::Wall;

                int wall_count = tl + tr + bl + br;

                // Skip corners with no walls or all walls (interior corners)
                if (wall_count == 0 || wall_count == 4)
                    continue;

                // Calculate normal based on wall configuration
                Vector2Float normal(0, 0);

                // Accumulate normals pointing away from walls
                if (tl) normal += Vector2Float(1, 1); // Push right-down
                if (tr) normal += Vector2Float(-1, 1); // Push left-down
                if (bl) normal += Vector2Float(1, -1); // Push right-up
                if (br) normal += Vector2Float(-1, -1); // Push left-up

                if (normal.sqrLength() > 0) {
                    normal = normal.normalized();
                    Vector2Int world_position(corner_x * tileWorldSize, corner_y * tileWorldSize);
                    func(world_position, normal);
                }
            }
        }
    }

    void cornerNormalsDisplacement(ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        auto AABB = convex->getBoundingBox();

        Vector2Float displacement = {0,0};

        iterateCornerNormals(AABB,space_ship,[&convex,&displacement](Vector2Int corner_position,Vector2Float normal) {
            if (!convex->is_inside(corner_position))
                return;

            Vector2Int diff = corner_position - convex->getCenter();

            Vector2Float direction = Vectors::toVector2Float(diff).normalized();

            Vector2Int end = Vectors::toVector2Int(direction*10000); //raycast like

            auto to_edge = Physics::localSegmentCast(convex->getCenter(),end,convex->getVertices());

            if (!to_edge.has_value()) //this should not happen
                return;

            float hit_distance_to_center = diff.length();
            float original_distance = (to_edge.value() - convex->getCenter()).length();
            float penetration = original_distance - hit_distance_to_center;

            if (penetration > 0) {
                // Push away from the wall along the ray direction
                displacement += -direction.normalized() * penetration;
            }
        });
        convex->owner_entity->movePosition(displacement, space_ship);
    }

    void visitConvexWall(ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        diagonalsDisplacement(convex, space_ship);
        cornerNormalsDisplacement(convex, space_ship);
    }
}
