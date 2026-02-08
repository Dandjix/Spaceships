//
// Created by timon on 2/8/26.
//

#pragma once
#include "Vectors.h"


namespace Scaling {
    // Scale to world --------------------------------------------------------------------------------------------------
    template<typename T>
    T scaleToWorld(T number) {
        return number * static_cast<T>(Vectors::getFactor());
    }

    template<typename T1, typename T2>
    Vector2<T1> scaleToWorld(Vector2<T2> screen_vector) {
        return Vector2<T1>(
            screen_vector.x * static_cast<T1>(Vectors::getFactor()),
            screen_vector.y * static_cast<T1>(Vectors::getFactor())
        );
    }

    // Scale to screen -------------------------------------------------------------------------------------------------
    template<typename T>
    T scaleToScreen(T number) {
        return number / static_cast<T>(Vectors::getFactor());
    }

    template<typename T1, typename T2>
    Vector2<T1> scaleToScreen(Vector2<T2> world_vector) {
        return Vector2<T1>(
            world_vector.x / static_cast<T1>(Vectors::getFactor()),
            world_vector.y / static_cast<T1>(Vectors::getFactor())
        );
    }

    // Metric (maybe Imperial later) translations ----------------------------------------------------------------------
#define UNITS_PER_METER 64.0f

    template<typename T>
    int metricToWorld(T meters) {
        return static_cast<int>(std::round(meters * static_cast<float>(Vectors::getFactor()) * UNITS_PER_METER));
    }

    template<typename T>
    Vector2Int metricToWorld(Vector2<T> metric_vector) {
        return {
            static_cast<int>(std::round(metric_vector.x * static_cast<float>(Vectors::getFactor()) * UNITS_PER_METER)),
            static_cast<int>(std::round(metric_vector.y * static_cast<float>(Vectors::getFactor()) * UNITS_PER_METER))
        };
    }
}
