//
// Created by timon on 2/9/26.
//

#include "BoundingBoxes.h"

BoundingBox<int> Physics::Util::createConvexBoundingBox(const std::vector<Vector2Int> &world_vertices) {
    // Define corners in local space

    int min_x = std::numeric_limits<int>::max(), max_x = std::numeric_limits<int>::min(), min_y = std::numeric_limits<
        int>::max(), max_y = std::numeric_limits<int>::min();

    for (auto c: world_vertices) {
        if (c.x < min_x)
            min_x = c.x;
        if (c.x > max_x)
            max_x = c.x;
        if (c.y < min_y)
            min_y = c.y;
        if (c.y > max_y)
            max_y = c.y;
    }

    return BoundingBox<int>({min_x, min_y}, {max_x - min_x, max_y - min_y});
}

BoundingBox<int> Physics::Util::createRoundBoundingBox(Vector2Int center, int radius) {
    Vector2Int TL;

    TL = center - Vector2Int(radius, radius);

    Vector2Int dimensions = {static_cast<int>(std::ceil(radius * 2)), static_cast<int>(std::ceil(radius * 2))};

    return {TL, dimensions};
}
