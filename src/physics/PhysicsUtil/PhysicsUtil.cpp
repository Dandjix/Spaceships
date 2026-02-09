//
// Created by timon on 2/9/26.
//

#include "PhysicsUtil.h"

#include "physics/SegmentCast.h"
#include "physics/SegmentCircleCast.h"

std::vector<std::pair<Vector2Int, Vector2Int>> Physics::Util::getDiagonals(Vector2Int center,
                                                                           const std::vector<Vector2Int> &polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > diagonals;
    diagonals.reserve(polygon.size());

    for (auto &i: polygon) {
        diagonals.emplace_back(center, i);
    }
    return diagonals;
}

std::vector<std::pair<Vector2Int, Vector2Int>> Physics::Util::getSides(std::vector<Vector2Int> polygon) {
    std::vector<std::pair<Vector2Int, Vector2Int> > sides;
    sides.reserve(polygon.size());

    for (int i = 0; i < polygon.size(); ++i) {
        std::size_t next_i = (i + 1) % polygon.size();
        sides.emplace_back(polygon[i], polygon[next_i]);
    }
    return sides;
}

bool Physics::Util::circleConvexAreColliding(const PolygonInfo &convex, const CircleInfo &round) {
    //if the convex's center is inside the circle, they are of course colliding.
    if ((convex.getCenter() - round.getCenter()).sqrLength() <= static_cast<float>(
            round.radius * round.radius))
        return true;

    auto sides = getSides(convex.vertices);

    return std::ranges::any_of(sides, [&](const auto &side) {
        auto [start, end] = side;
        return Physics::segmentCircleIntersectionFloat(start, end, round.getCenter(), round.radius).
                has_value();
    });
}

bool Physics::Util::areCollidingSAT(const PolygonInfo *poly1, const PolygonInfo *poly2) {
    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }

        auto p1 = poly1->vertices;

        for (auto [edge_start,edge_end]: getSides(poly1->vertices)) {
            Vector2Float normal = Vector2Float(static_cast<float>(-edge_end.y + edge_start.y),
                                               static_cast<float>(edge_end.x - edge_start.x));

            float min_r1 = INFINITY, max_r1 = -INFINITY;
            for (auto vertex: poly1->vertices) {
                float q = static_cast<float>(vertex.x) * normal.x + static_cast<float>(vertex.y) * normal.y;
                // dot product
                min_r1 = std::min(min_r1, q);
                max_r1 = std::max(max_r1, q);
            }

            float min_r2 = INFINITY, max_r2 = -INFINITY;
            for (auto vertex: poly2->vertices) {
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

bool Physics::Util::areCollidingDiagonals(const PolygonInfo *poly1, const PolygonInfo *poly2) {
    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }
        auto diagonals_1 = getDiagonals(poly1->getCenter(), poly1->vertices);
        // auto diagonals_2 = getDiagonals(poly2->getCenter(), poly2->vertices);
        // auto sides_1 = getSides(poly1->vertices);
        auto sides_2 = getSides(poly2->vertices);
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

Physics::Util::SATReturn Physics::Util::SeparatedAxisTheorem(PolygonInfo *poly1, PolygonInfo *poly2) {
    float overlap = INFINITY;

    for (int shape = 0; shape < 2; shape++) {
        if (shape == 1) {
            auto temp = poly1;
            poly1 = poly2;
            poly2 = temp;
        }

        auto p1 = poly1->vertices;

        for (auto [edge_start,edge_end]: getSides(poly1->vertices)) {
            Vector2Float normal = Vector2Float(-edge_end.y + edge_start.y, edge_end.x - edge_start.x).
                    normalized();

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
