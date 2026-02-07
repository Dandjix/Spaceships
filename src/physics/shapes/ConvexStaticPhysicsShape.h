#pragma once
#include "ConvexPhysicsShape.h"
//
// Created by timon on 2/7/26.
//


class ConvexStaticPhysicsShape : public ConvexPhysicsShape{
public:
    ConvexStaticPhysicsShape(PhysicsEntity *owner_entity, const std::vector<Vector2Int> &vertices)
        : ConvexPhysicsShape(owner_entity, vertices) {
    }

    PhysicsUpdateVisitor * createVisitor() override;

    void consumeVisitor(PhysicsUpdateVisitor *visitor, SpaceShip *space_ship) override;
};