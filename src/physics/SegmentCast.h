#pragma once
#include <optional>

#include "math/Vectors.h"
//
// Created by timon on 2/6/26.
//


namespace Physics {
    [[nodiscard]] std::optional<Vector2Int> localSegmentCast(
        Vector2Int start,
        Vector2Int end,
        const std::vector<Vector2Int> &world_vertices
    );

    [[nodiscard]] std::optional<Vector2Int> segmentIntersection(
        Vector2Int start_1, Vector2Int end_1,
        Vector2Int start_2, Vector2Int end_2
    );


    struct segmentIntersectionFloatsResult {
        float t1;
        float t2;
        bool success;
    };

    /**
     * This returns the progress of the intersection
     * @param start_1
     * @param end_1
     * @param start_2
     * @param end_2
     * @return
     */
    [[nodiscard]] segmentIntersectionFloatsResult segmentIntersectionFloats(
        Vector2Int start_1, Vector2Int end_1,
        Vector2Int start_2, Vector2Int end_2
    );

};
