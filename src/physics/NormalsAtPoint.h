#pragma once
#include "math/Vectors.h"
//
// Created by timon on 2/6/26.
//


class ConvexPhysicsShape;

namespace Physics {
    Vector2Float getEdgeNormalAtContactPoint(ConvexPhysicsShape* convex, Vector2Int contact_point, Vector2Int other_center);

    bool isPointOnSegment(Vector2Int point, Vector2Int seg_start, Vector2Int seg_end);
}
