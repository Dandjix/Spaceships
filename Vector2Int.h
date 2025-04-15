#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <cmath> 

struct Vector2Int {
    int x, y;

    Vector2Int(int x = 0, int y = 0) : x(x), y(y) {}

    float length() const {
        return std::sqrtf(static_cast<float>(x * x + y * y));
    }

    int sqrLength() const {
        return x * x + y * y;
    }

    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x = static_cast<int> (x/len);
            y = static_cast<int> (y/len);
        }
    }

    Vector2Int operator*(float scalar) const {
        return Vector2Int(static_cast<int>(x * scalar), static_cast<int>(y * scalar));
    }

    int operator*(Vector2Int& other) const {
        return x * other.x + y * other.y;
    }

    Vector2Int operator+(Vector2Int& other) const {
        return Vector2Int(x + other.x, y + other.y);
    }
};
