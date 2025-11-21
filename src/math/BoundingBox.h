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
};
