#pragma once
#include "Vector2Float.h"
#include "Vector2Int.h"

constexpr float factor = 8;
// un facteur de 1 -> 1px = 1 unité de distance
// facteur 1024 -> 1px = 1024 unités de distance

static Vector2Float toVector2Float(const Vector2Int& vec)
{
    return Vector2Float(floor(vec.x+0.5f) / factor, floor(vec.y+0.5f) / factor);
}

static Vector2Int toVector2Int(const Vector2Float& vec)
{
    return Vector2Int(static_cast<int>(vec.x * factor), static_cast<int>(vec.y * factor));
}