#pragma once
#include <vector>

#include "math/Vectors.h"
//
// Created by timon on 2/9/26.
//

class SpaceShip;
class PhysicsShape;

namespace Physics {
    std::vector<PhysicsShape *> ConvexCast(std::vector<Vector2Int> points, SpaceShip *space_ship);
}
