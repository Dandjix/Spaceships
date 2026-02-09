//
// Created by timon on 11/21/25.
//

#pragma once
#include "Vectors.h"
#include "math/Scaling.h"
#include "spaceships/Tile.h"

template<typename T>
class BoundingBox {
public:
    Vector2<T> topLeftCorner;
    Vector2<T> boundingBoxDimensions;

    BoundingBox(Vector2<T> topLeftCorner, Vector2<T> boundingBoxDimensions)
        : topLeftCorner(topLeftCorner), boundingBoxDimensions(boundingBoxDimensions) {
    }

    Vector2<T> topLeft() const {
        return topLeftCorner;
    }

    Vector2<T> bottomRight() const {
        return topLeftCorner + boundingBoxDimensions;
    }

    Vector2<T> dimensions() const {
        return boundingBoxDimensions;
    }

    bool intersects(BoundingBox other) const {
        Vector2<T> aMin = topLeftCorner;
        Vector2<T> aMax = topLeftCorner + boundingBoxDimensions;

        Vector2<T> bMin = other.topLeftCorner;
        Vector2<T> bMax = other.topLeftCorner + other.boundingBoxDimensions;

        // No overlap if one is completely to the left/right or above/below the other
        if (aMax.x <= bMin.x) return false; // this is left of other
        if (bMax.x <= aMin.x) return false; // other is left of this
        if (aMax.y <= bMin.y) return false; // this is above other
        if (bMax.y <= aMin.y) return false; // other is above this

        return true;
    }

    [[nodiscard]] std::vector<Vector2Int> encompassedTiles() const {
        int start_x = topLeft().x / Scaling::scaleToWorld(Tiles::tileSizePx);
        int start_y = topLeft().y / Scaling::scaleToWorld(Tiles::tileSizePx);

        int end_x = (bottomRight().x-1) / Scaling::scaleToWorld(Tiles::tileSizePx);
        int end_y = (bottomRight().y-1) / Scaling::scaleToWorld(Tiles::tileSizePx);

        int x_dimension = (end_x - start_x + 1) * (end_x - start_x + 1);
        int y_dimension = (end_y - start_y + 1) * (end_y - start_y + 1);
        std::vector<Vector2Int> tiles;
        tiles.reserve(x_dimension * y_dimension);
        for (int i = start_x; i <= end_x; ++i) {
            for (int j = start_y; j <= end_y; ++j) {
                tiles.push_back({i,j});
            }
        }

        return tiles;
    }

    bool is_inside(Vector2Int world_position) const {
        return
                (world_position.x >= topLeftCorner.x && world_position.x <= topLeftCorner.x + boundingBoxDimensions.x)
                &&
                (world_position.y >= topLeftCorner.y && world_position.y <= topLeftCorner.y + boundingBoxDimensions.y);
    }
};
