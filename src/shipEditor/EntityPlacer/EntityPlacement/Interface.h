#pragma once
#include "math/Color.h"
#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//

namespace EntityPlacement {
    class Interface {
    protected:
        Vector2Int position_to_place;
        float angle_to_place;

    public:
        [[nodiscard]] Vector2Int getPosition() const { return position_to_place; }

        [[nodiscard]] float getAngle(float snap = 0) const {
            if (snap == 0) return angle_to_place;

            return std::fmod(angle_to_place, snap);
        }

        void setPositionToPlace(Vector2Int position_to_place_param) { position_to_place = position_to_place_param; }

        void setAngleToPlace(float angle_to_place_param) { angle_to_place = angle_to_place_param; }

        Interface()
            : position_to_place({0, 0}), angle_to_place(0) {
        }

        // template<typename T, typename T2>
        // T askFor() {
        //
        // }
    };
};
