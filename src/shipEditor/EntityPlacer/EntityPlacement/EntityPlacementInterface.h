#pragma once
#include "math/Color.h"
#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//

namespace EntityPlacement {
    class EntityPlacementInterface {
    protected:
        Vector2Int position;
        float angle;

    public:
        [[nodiscard]] Vector2Int getPosition() const {return position;}

        [[nodiscard]] float getAngle() const {return angle;}

        EntityPlacementInterface(Vector2Int world_position, float world_angle)
            : position(world_position), angle(world_angle) {
        }

        // template<typename T, typename T2>
        // T askFor() {
        //
        // }
    };
};
