#pragma once
#include "ConvexStaticPhysicsShape.h"
//
// Created by timon on 2/7/26.
//


class RectStaticPhysicsShape : public ConvexStaticPhysicsShape{
public:
    RectStaticPhysicsShape(PhysicsEntity *owner_entity, Vector2Int dimensions)
        : ConvexStaticPhysicsShape(
            owner_entity,
            {
                {-dimensions.x / 2, -dimensions.y / 2},
                {dimensions.x / 2, -dimensions.y / 2},
                {dimensions.x / 2, dimensions.y / 2},
                {-dimensions.x / 2, dimensions.y / 2}
            }
        ) {
    }
};