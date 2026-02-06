//
// Created by timon on 2/6/26.
//

#include "NormalsAtPoint.h"

#include "shapes/ConvexPhysicsShape.h"

Vector2Float Physics::getEdgeNormalAtContactPoint(ConvexPhysicsShape *convex, Vector2Int contact_point,
                                                  Vector2Int other_center) {
    auto vertices = convex->getVertices();

    // Find which edge contains the contact point
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2Int edge_start = vertices[i];
        Vector2Int edge_end = vertices[(i + 1) % vertices.size()];

        // Check if contact_point is on this edge (within tolerance)
        if (isPointOnSegment(contact_point, edge_start, edge_end)) {
            // Edge direction vector
            Vector2Float edge_dir = Vectors::toVector2Float(edge_end - edge_start).normalized();

            // Normal is perpendicular to edge (rotate 90°)
            Vector2Float normal1 = Vector2Float{-edge_dir.y, edge_dir.x};
            Vector2Float normal2 = Vector2Float{edge_dir.y, -edge_dir.x};

            // Choose the normal pointing toward the other object
            Vector2Float to_other = Vectors::toVector2Float(other_center - contact_point);
            if (normal1.dot(to_other) > 0)
                return normal1;
            else
                return normal2;
        }
    }

    // Fallback: use direction from convex center to round center
    return Vectors::toVector2Float(other_center - convex->getCenter()).normalized();
}

bool Physics::isPointOnSegment(Vector2Int point, Vector2Int seg_start, Vector2Int seg_end) {
    // Check if point is collinear and within segment bounds
    Vector2Int v1 = point - seg_start;
    Vector2Int v2 = seg_end - seg_start;

    float cross = std::abs(v1.cross(v2));
    if (cross > 1.0f) return false; // Not collinear (tolerance for int precision)

    float dot = v1.dot(v2);
    float len_sq = v2.sqrLength();

    return dot >= 0 && dot <= len_sq;
}
