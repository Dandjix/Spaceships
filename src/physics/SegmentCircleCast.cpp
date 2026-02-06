//
// Created by timon on 2/6/26.
//

#include "SegmentCircleCast.h"

#include <format>

float toFloatSquared(int v) {
    return static_cast<float>(v)*static_cast<float>(v);
}

float toFloatMultiplied(int x,int y) {
    return static_cast<float>(x)*static_cast<float>(y);
}

int roundToInt(float v) {
    return static_cast<int>(std::round(v));
}

std::optional<Vector2Int> Physics::segmentCircleIntersection(
    Vector2Int segment_start,
    Vector2Int segment_end,
    Vector2Int circle_center,
    int circle_radius
) {
    // Direction vector: d = end - start
    int dx = segment_end.x - segment_start.x;
    int dy = segment_end.y - segment_start.y;

    // Start relative to center: f = start - center
    int fx = segment_start.x - circle_center.x;
    int fy = segment_start.y - circle_center.y;



    // Quadratic coefficients: at² + bt + c = 0
    float a = toFloatSquared(dx) + toFloatSquared(dy);
    float b = 2 * toFloatMultiplied(fx,dx) + toFloatMultiplied(fy,dy);
    float c = toFloatSquared( fx) + toFloatSquared( fy) - toFloatSquared( circle_radius);

    // Check discriminant
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt; // No intersection
    }

    // Compute sqrt(discriminant) using integer square root
    float sqrt_disc = 0;
    if (discriminant > 0) {
        float left = 0, right = discriminant;
        while (left <= right) {
            float mid = left + (right - left) / 2;
            if (mid * mid <= discriminant) {
                sqrt_disc = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    // Two potential t values (scaled by 2a to avoid division)
    // t = (-b ± sqrt_disc) / (2a)
    // We check: t_scaled = -b ± sqrt_disc, and verify 0 <= t_scaled <= 2a

    float t1_scaled = -b - sqrt_disc;
    float t2_scaled = -b + sqrt_disc;

    // Choose the smallest valid t (closest to segment_start)
    float t_scaled = -1.0f;

    if (t1_scaled >= 0 && t1_scaled <= 2 * a) {
        t_scaled = t1_scaled;
    } else if (t2_scaled >= 0 && t2_scaled <= 2 * a) {
        t_scaled = t2_scaled;
    }

    if (t_scaled < 0) {
        return std::nullopt; // No intersection on segment
    }

    // Compute intersection point: P = start + t * d
    // t = t_scaled / (2a)
    // P = start + (t_scaled / 2a) * d = start + (t_scaled * d) / (2a)

    int x = segment_start.x + roundToInt((t_scaled * dx) / (2 * a));
    int y = segment_start.y + roundToInt((t_scaled * dy) / (2 * a));

    return Vector2Int{x, y};
}
