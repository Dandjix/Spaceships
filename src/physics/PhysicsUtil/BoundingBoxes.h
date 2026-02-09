#pragma once
#include "math/BoundingBox.h"
//
// Created by timon on 2/9/26.
//


namespace Physics {
    namespace Util {
        BoundingBox<int> createConvexBoundingBox(const std::vector<Vector2Int> & world_vertices);

        BoundingBox<int> createRoundBoundingBox(Vector2Int center, int radius);

    }
};