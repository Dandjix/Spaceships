//
// Created by timon on 11/21/25.
//

#pragma once
#include "Vectors.h"


template<typename T>
class BoundingBox
{
public:
    Vector2<T> topLeftCorner;
    Vector2<T> boundingBoxDimensions;
    BoundingBox(Vector2<T> topLeftCorner, Vector2<T> boundingBoxDimensions)
    :
    topLeftCorner(topLeftCorner),boundingBoxDimensions(boundingBoxDimensions)
    {}

    Vector2<T> topLeft()
    {
        return topLeftCorner;
    }

    Vector2<T> bottomRight()
    {
        return topLeftCorner + boundingBoxDimensions;
    }

    Vector2<T> dimensions()
    {
        return boundingBoxDimensions;
    }

    bool intersects(BoundingBox other)
    {
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

    bool is_inside(Vector2Int world_position) {
        return
        (world_position.x >= topLeftCorner.x && world_position.x <= topLeftCorner.x + boundingBoxDimensions.x)
        &&
        (world_position.y >= topLeftCorner.y && world_position.y <= topLeftCorner.y + boundingBoxDimensions.y);

    }
};
