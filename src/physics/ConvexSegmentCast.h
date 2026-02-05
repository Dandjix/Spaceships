#pragma once
#include <optional>

#include "math/Vectors.h"
//
// Created by timon on 2/6/26.
//


namespace Physics {
    std::optional<Vector2Int> localSegmentCast(
        Vector2Int start,
        Vector2Int end,
        const std::vector<Vector2Int> &world_vertices
    );

    std::optional<Vector2Int> segmentIntersection(
        Vector2Int start_1, Vector2Int end_1,
        Vector2Int start_2, Vector2Int end_2
    );
};
