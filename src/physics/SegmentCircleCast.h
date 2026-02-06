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

    /**
     * @param segment_start
     * @param segment_end
     * @param circle_center
     * @param circle_radius
     * @return a float in the range [0,1] representing the progress along the segment at which the intersection occurs,
     * std::nullopt if no intersection
     */
    std::optional<float> segmentCircleIntersectionFloat(
        Vector2Int segment_start, Vector2Int segment_end,
        Vector2Int circle_center, int circle_radius
    );
};