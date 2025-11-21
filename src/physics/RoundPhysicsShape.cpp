//
// Created by timon on 11/21/25.
//

#include "RoundPhysicsShape.h"

BoundingBox<int> RoundPhysicsShape::getBoundingBox()
{
    Vector2Int TL;

    TL = owner_entity->getPosition() - Vector2Int( radius,radius);

    Vector2Int dimensions = {static_cast<int>(std::ceilf(radius*2)),static_cast<int>(std::ceilf(radius*2))};

    return BoundingBox<int>(TL,dimensions);
}
