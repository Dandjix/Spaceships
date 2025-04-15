#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <cmath> 

struct Vector2Float {
    float x, y;

    Vector2Float(float x = 0, float y = 0) : x(x), y(y) {}

    float length() const {
        return std::sqrtf(x * x + y * y);
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

    Vector2Float operator*(float scalar) const {
        return Vector2Float(x * scalar, y * scalar);
    }

    float operator*(const Vector2Float& other) const {
        return x * other.x + y * other.y;
    }

    Vector2Float operator+(const Vector2Float& other) const {
        return Vector2Float(x + other.x, y + other.y);
    }

    Vector2Float operator-(const Vector2Float& other) const {
        return Vector2Float(x - other.x, y - other.y);
    }
};
