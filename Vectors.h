#pragma once
#include <cmath>
#include <iostream>


template <typename T>
struct Vector2 {
    T x, y;

    Vector2(T x = 0, T y = 0) : x(x), y(y) {}

    float length() const {
        return std::sqrt(static_cast<float>(x * x + y * y));
    }

    T sqrLength() const {
        return x * x + y * y;
    }

    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x = static_cast<T>(x / len);
            y = static_cast<T>(y / len);
        }
    }

    Vector2<T> operator*(float scalar) const {
        return Vector2<T>(static_cast<T>(x * scalar), static_cast<T>(y * scalar));
    }

    T operator*(const Vector2<T>& other) const {
        return x * other.x + y * other.y;
    }

    Vector2<T> operator+(const Vector2<T>& other) const {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> operator-(const Vector2<T>& other) const {
        return Vector2<T>(x - other.x, y - other.y);
    }

    bool operator==(const Vector2<T>& other) const {
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};


using Vector2Int = Vector2<int>;
using Vector2Float = Vector2<float>;

inline const int factor = 8;
// sous division des pixels
// un facteur de 1 -> 1px = 1 unité de distance
// facteur 1024 -> 1px = 1024 unités de distance

class Vectors
{
public:
    static Vector2Float toVector2Float(const Vector2Int& vec, bool preserveScale = false)
    {
        if (preserveScale)
        {
            return Vector2Float(static_cast<float>(vec.x), static_cast<float>(vec.y));
        }
        return Vector2Float(static_cast<float>(vec.x) / factor, static_cast<float>(vec.y) / factor);
    }

    static Vector2Int toVector2Int(const Vector2Float& vec, bool preserveScale = false)
    {
        if (preserveScale)
        {
            return Vector2Int(static_cast<int>(vec.x), static_cast<int>(vec.y));
        }
        return Vector2Int(static_cast<int>(vec.x * factor), static_cast<int>(vec.y * factor));
    }

    static Vector2Int scaleToInt(float x, float y)
    {
        return Vector2Int(static_cast<int>(x * factor), static_cast<int>(y * factor));
    }

    static Vector2Int scaleIntVector(int x, int y)
    {
        return Vector2Int(x * factor, y * factor);
    }

    static Vector2Int scaleIntVector(Vector2Int position)
    {
        return Vector2Int(position.x * factor, position.y * factor);
    }

};

