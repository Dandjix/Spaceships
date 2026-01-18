#pragma once
#include <cmath>
#include <iostream>
#include <functional>

#include "json.hpp"
#include "json.hpp"

inline const int factor = 64;
// pixels subdivision
// factor 1 : 1px = 1 distance unit
// factor 1024 -> 1px = 1024 distance units

template<class T>
inline void hash_combine(std::size_t &s, const T &v) {
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

template<typename T>
struct Vector2 {
    T x, y;

    Vector2(T x = 0, T y = 0) : x(x), y(y) {
    }

    [[nodiscard]] float length() const {
        float l = std::sqrt(
            static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y));
        if (std::isnan(l)) {
            throw std::runtime_error("l is nan ! x : " + std::to_string(x) + ", y : " + std::to_string(y));
        }
        return l;
    }

    [[nodiscard]] float sqrLength() const {
        return static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y);
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

    Vector2<T> operator/(float divider) const {
        return Vector2<T>(static_cast<T>(x / divider), static_cast<T>(y / divider));
    }

    Vector2<T> operator%(T modulus) {
        return Vector2<T>(x % modulus, y % modulus);
    }

    T operator*(const Vector2<T> &other) const {
        return x * other.x + y * other.y;
    }

    Vector2<T> operator+(const Vector2<T> &other) const {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> operator-(const Vector2<T> &other) const {
        return Vector2<T>(x - other.x, y - other.y);
    }

    bool operator==(const Vector2<T> &other) const {
        return x == other.x && y == other.y;
    }

    /**
     * rotates the vector
     * @param degrees the angle in degrees
     * @return the rotated vector
     */
    [[nodiscard]] Vector2<T> rotate(float degrees) const {
        if constexpr (std::is_same_v<T, float>) {
            float radians = degrees * (3.14159265358979323846f / 180.0f);
            float cosB = cosf(radians);
            float sinB = sinf(radians);
            T x2 = cosB * x - sinB * y;
            T y2 = sinB * x + cosB * y;
            return Vector2<T>(x2, y2);
        } else if constexpr (std::is_same_v<T, int>) {
            constexpr int64_t SCALE = 1 << 16;

            float radians = degrees * (3.14159265358979323846f / 180.0f);
            auto cosF = static_cast<int64_t>(cosf(radians) * SCALE);
            auto sinF = static_cast<int64_t>(sinf(radians) * SCALE);
            T x2 = (cosF * x - sinF * y) >> 16;
            T y2 = (sinF * x + cosF * y) >> 16;
            return Vector2<T>( x2, y2 );
        } else {
            static_assert(std::is_same_v<T, float>, "rotate() is only defined for floats and ints");
        }
    }

    /// <summary>
    /// scales the screen position up to a world position.
    /// </summary>
    /// <returns></returns>
    [[nodiscard]] Vector2<T> scaleToWorldPosition() const {
        return Vector2<T>(x * factor, y * factor);
    }

    /// <summary>
    /// scales the world position down to a screen position
    /// </summary>
    /// <returns></returns>
    [[nodiscard]] Vector2<T> scaleToScreenPosition() const {
        return Vector2<T>(x / factor, y / factor);
    }


    friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    Vector2<T> &operator/=(float divider) {
        x = static_cast<T>(x / divider);
        y = static_cast<T>(y / divider);
        return *this;
    }

    Vector2<T> &operator*=(float scalar) {
        x = static_cast<T>(x * scalar);
        y = static_cast<T>(y * scalar);
        return *this;
    }

    Vector2<T> &operator+=(const Vector2 &vector2) {
        x = static_cast<T>(x + vector2.x);
        y = static_cast<T>(y + vector2.y);
        return *this;
    }

    Vector2<T> &operator-=(const Vector2 &vector2) {
        x = static_cast<T>(x - vector2.x);
        y = static_cast<T>(y - vector2.y);
        return *this;
    }

    Vector2<T> operator%=(T modulus) {
        x = x % modulus;
        y = y % modulus;
        return *this;
    }

    Vector2<T> operator-() {
        return {-x, -y};
    }

    [[nodiscard]] Vector2<float> normalized() const {
        Vector2<float> result = *this;
        result.normalize();
        return result;
    }

    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json entry = {};
        entry["x"] = x;
        entry["y"] = y;
        return entry;
    }

    [[nodiscard]]static Vector2<T> lerp(Vector2<T> a, Vector2<T> b, float c) {

        T x = static_cast<T>(static_cast<float>(a.x)*(1-c)+static_cast<float>(b.x)*(c));
        T y = static_cast<T>(static_cast<float>(a.y)*(1-c)+static_cast<float>(b.y)*(c));

        return Vector2<T>(
            x,
            y
        );
    }

    static Vector2<T> fromJson(nlohmann::json entry) {
        T x = entry["x"];
        T y = entry["y"];
        return Vector2<T>(x, y);
    }
};


using Vector2Int = Vector2<int>;
using Vector2Float = Vector2<float>;

template<typename Tin, typename Tout>
class Vector2Translations {
public:
    static Vector2<Tout> convert(Vector2<Tin> source) {
        return Vector2<Tout>(
            static_cast<Tout>(source.x),
            static_cast<Tout>(source.y)
        );
    }
};

class Vectors {
public:
    static int getFactor() {
        return factor;
    }

    /**
     * translates a vector int to float. Does not apply the factor.
     * @param vec
     * @return
     */
    static Vector2Float toVector2Float(const Vector2Int &vec) {
        return Vector2Translations<int, float>::convert(vec);
    }

    /**
     * translates a vector float to int. Does not apply the factor.

     * @param vec
     * @return
     */
    static Vector2Int toVector2Int(const Vector2Float &vec) {
        return Vector2Translations<float, int>::convert(vec);
    }
};

namespace std {
    template<>
    struct hash<Vector2Int> {
        std::size_t operator()(const Vector2Int &v) const noexcept {
            std::size_t seed = 0;
            hash_combine(seed, v.x);
            hash_combine(seed, v.y);
            return seed;
        }
    };
}


namespace Scaling {
    template<typename T>
    T scaleToWorld(T number) {
        return number * static_cast<T>(Vectors::getFactor());
    }

    template<typename T>
    T scaleToScreen(T number) {
        return number / static_cast<T>(Vectors::getFactor());
    }
}
