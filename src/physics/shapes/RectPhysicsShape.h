//
// Created by timon on 11/18/25.
//

#pragma once
#include "ConvexPhysicsShape.h"
#include "../scripts/PhysicsEntity.h"
#include "PhysicsShape.h"
#include "../PhysicsUpdateVisitor/PhysicsUpdateVisitorConvex.h"


class RectPhysicsShape : public ConvexPhysicsShape {
public:
    /**
     * The dimensions of the shape in world units
     */
public:
    RectPhysicsShape(PhysicsEntity *owner_entity, Vector2Int dimensions)
        : ConvexPhysicsShape(
            owner_entity,
            {
                {-dimensions.x / 2, -dimensions.y / 2},
                {dimensions.x / 2, -dimensions.y / 2},
                {dimensions.x / 2, dimensions.y / 2},
                {-dimensions.x / 2, dimensions.y / 2}
            }
        ) {
    }

    void setDimensions(Vector2Int dimensions) {
        setRawVertices( {
            {-dimensions.x / 2, -dimensions.y / 2},
            {dimensions.x / 2, -dimensions.y / 2},
            {dimensions.x / 2, dimensions.y / 2},
            {-dimensions.x / 2, dimensions.y / 2}
        });
    }
};
