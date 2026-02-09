//
// Created by timon on 11/18/25.

#include "PhysicsCollisions.h"


#include "math/Vectors.h"
#include "../shapes/RoundPhysicsShape.h"
#include "physics/Physics.h"
#include "physics/SegmentCast.h"
#include "physics/PhysicsUtil/PhysicsUtil.h"
#include "physics/shapes/ConvexPhysicsShape.h"
#include "physics/shapes/ConvexStaticPhysicsShape.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/Tile.h"
/**
 * This is applied to two entities that are at the same position to unstuck them
 */
constexpr float jolt = 3;

namespace PhysicsCollisions {
    void applyJolt(PhysicsShape *shape1, PhysicsShape *shape2, SpaceShip *space_ship) {
        float random_angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 360;

        Vector2Float jolt_vector = Vector2Float(jolt, 0.0f).rotate(random_angle);

        shape1->moveCenter(jolt_vector);
        shape2->moveCenter(-jolt_vector);
    }


    void visitConvexes(ConvexPhysicsShape *poly1, ConvexPhysicsShape *poly2, SpaceShip *space_ship) {
        Physics::Util::SATReturn res;
        {
            auto poly1_info = Physics::Util::PolygonInfo(poly1->getVertices());
            auto poly2_info = Physics::Util::PolygonInfo(poly2->getVertices());
            res = SeparatedAxisTheorem(&poly1_info, &poly2_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(poly1->getCenter() - poly2->getCenter()).normalized() * res.
                             overlap;

        float e1_weight = poly1->getWeight();
        float e2_weight = poly2->getWeight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        poly1->moveCenter(delta * force_e1);
        poly2->moveCenter(-delta * force_e2);
    }

    // here's a little lesson in trickery, this is going down in history
    void visitConvexRound(ConvexPhysicsShape *convex, RoundPhysicsShape *round, SpaceShip *space_ship) {
        Physics::Util::SATReturn res;
        {
            auto convex_info = Physics::Util::PolygonInfo(convex->getVertices());
            auto round_info = Physics::Util::PolygonInfo(round->getVertices());
            res = SeparatedAxisTheorem(&convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(convex->getCenter() - round->getCenter()).normalized() * res.
                             overlap;

        float e1_weight = convex->getWeight();
        float e2_weight = round->getWeight();

        float force_e1 = (e2_weight / (e2_weight + e1_weight));
        float force_e2 = (e1_weight / (e2_weight + e1_weight));

        convex->moveCenter(delta * force_e1);
        round->moveCenter(-delta * force_e2);
    }

    void visitStaticRoundConvex(RoundStaticPhysicsShape *static_round, ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        Physics::Util::SATReturn res;
        {
            auto convex_info = Physics::Util::PolygonInfo(convex->getVertices());
            auto round_info = Physics::Util::PolygonInfo(static_round->getVertices());
            res = SeparatedAxisTheorem(&convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(convex->getCenter() - static_round->getCenter()).normalized() * res.
                             overlap;

        convex->moveCenter(delta);
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

        float e1_weight = shape1->getWeight();
        float e2_weight = shape2->getWeight();

        float force_e1 = force_value * (e2_weight / (e2_weight + e1_weight));
        float force_e2 = force_value * (e1_weight / (e2_weight + e1_weight));

        auto delta_e1 = (Vectors::toVector2Float(
                             shape1->getCenter() - shape2->getCenter()).normalized() *
                         force_e1);
        auto delta_e2 = (Vectors::toVector2Float(
                             shape2->getCenter() - shape1->getCenter()).normalized() *
                         force_e2);

        shape1->moveCenter(delta_e1);
        shape2->moveCenter(delta_e2);
    }

    void visitStaticConvexConvex(ConvexStaticPhysicsShape *static_convex, ConvexPhysicsShape *convex,
        SpaceShip *space_ship) {
        Physics::Util::SATReturn res;
        {
            auto static_convex_info = Physics::Util::PolygonInfo(static_convex->getVertices());
            auto convex_info = Physics::Util::PolygonInfo(convex->getVertices());
            res = SeparatedAxisTheorem(&static_convex_info, &convex_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(static_convex->getCenter() - convex->getCenter()).normalized() * res.
                             overlap;

        convex->moveCenter(-delta);
    }

    void visitStaticConvexRound(ConvexStaticPhysicsShape *static_convex, RoundPhysicsShape *round,
        SpaceShip *space_ship) {
        Physics::Util::SATReturn res;
        {
            auto static_convex_info = Physics::Util::PolygonInfo(static_convex->getVertices());
            auto round_info = Physics::Util::PolygonInfo(round->getVertices());
            res = SeparatedAxisTheorem(&static_convex_info, &round_info);
        }

        if (!res.are_colliding)
            return;

        Vector2Float delta = Vectors::toVector2Float(static_convex->getCenter() - round->getCenter()).normalized() * res.
                             overlap;

        round->moveCenter(-delta);
    }

    void visitStaticRoundRound(RoundStaticPhysicsShape *shape1, RoundPhysicsShape *shape2, SpaceShip *space_ship) {
        if (shape1->getCenter() == shape2->getCenter()) {
            float random_angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 360;

            Vector2Float jolt_vector = Vector2Float(jolt, 0.0f).rotate(random_angle);

            shape2->moveCenter(jolt_vector);
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

        shape2->moveCenter(delta_e2);
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
                    shape1->moveCenter(delta);

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

        for (auto [start,end]: Physics::Util::getDiagonals(convex->getCenter(), convex->getVertices())) {
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
            convex->moveCenter(displacement / hit_count);
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
        convex->moveCenter(displacement);
    }

    void visitConvexWall(ConvexPhysicsShape *convex, SpaceShip *space_ship) {
        diagonalsDisplacement(convex, space_ship);
        cornerNormalsDisplacement(convex, space_ship);
    }
};