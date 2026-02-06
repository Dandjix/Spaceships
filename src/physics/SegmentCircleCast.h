#pragma once
#include <optional>

#include "math/Vectors.h"
//
// Created by timon on 2/6/26.
//


namespace Physics {
    std::optional<Vector2Int> segmentCircleIntersection(
        Vector2Int segment_start, Vector2Int segment_end,
        Vector2Int circle_center, int circle_radius
    );
    std::optional<float> segmentCircleIntersectionFloat(
        Vector2Int segment_start, Vector2Int segment_end,
        Vector2Int circle_center, int circle_radius
    );
};