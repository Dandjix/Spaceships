#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <cmath> // for sqrt

// Simple Vector2 struct
struct Vector2 {
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float sqrLength() const {
        return x * x + y * y;
    }

    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    float operator*(Vector2& other) const {
        return x * other.x + y * other.y;
    }

    Vector2 operator+(Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
};