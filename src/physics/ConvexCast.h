#pragma once
#include <vector>

#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//

namespace Instances {
    class Instance;
}

class SpaceShip;
class PhysicsShape;

namespace Physics {
    std::vector<PhysicsShape *> ConvexCast(std::vector<Vector2Int> points, Instances::Instance *instance);
    std::vector<PhysicsShape *> RectCast(Vector2Int rect_center, Vector2Int rect_dimensions, float angle, Instances::Instance *world_instance);
}
