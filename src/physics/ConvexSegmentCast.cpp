//
// Created by timon on 2/6/26.
//

#include "ConvexSegmentCast.h"

std::optional<Vector2Int> Physics::localSegmentCast(Vector2Int start, Vector2Int end,
    const std::vector<Vector2Int> &world_vertices) {
    if (world_vertices.size() < 3)
        return std::nullopt;

    std::optional<Vector2Int> closest_intersection = std::nullopt;
    float closest_distance_sq = std::numeric_limits<float>::max();

    // Check intersection with each edge
    for (size_t i = 0; i < world_vertices.size(); ++i) {
        Vector2Int edge_start = world_vertices[i];
        Vector2Int edge_end = world_vertices[(i + 1) % world_vertices.size()];

        auto intersection = segmentIntersection(start, end, edge_start, edge_end);

        if (intersection.has_value()) {
            Vector2Int point = intersection.value();
            float dist_sq = (point - start).sqrLength();

            if (dist_sq < closest_distance_sq) {
                closest_distance_sq = dist_sq;
                closest_intersection = point;
            }
        }
    }

    return closest_intersection;
}

std::optional<Vector2Int> Physics::segmentIntersection(Vector2Int start_1, Vector2Int end_1, Vector2Int start_2, Vector2Int end_2) {
    Vector2Int r = end_1 - start_1;  // Direction of first segment
    Vector2Int s = end_2 - start_2;  // Direction of second segment

    float r_cross_s = r.cross(s);
    Vector2Int p3_minus_p1 = start_2 - start_1;
    float p3_minus_p1_cross_r = p3_minus_p1.cross(r);

    // Parallel or collinear - no single intersection point
    if (std::abs(r_cross_s) < 1e-6f)
        return std::nullopt;

    float t = p3_minus_p1.cross(s) / r_cross_s;
    float u = p3_minus_p1_cross_r / r_cross_s;

    // Check if intersection is within both segments
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        // Calculate intersection point
        // Using integer arithmetic might cause precision issues
        // Consider using float intermediate values
        int x = start_1.x + static_cast<int>(t * r.x);
        int y = start_1.y + static_cast<int>(t * r.y);
        return Vector2Int{x, y};
    }

    return std::nullopt;
}

