#pragma once
#include "PhysicsShape.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorConvex.h"
//
// Created by timon on 2/2/26.
//


class ConvexPhysicsShape : public PhysicsShape {
protected:
    std::vector<Vector2Int> vertices;

public:
    explicit ConvexPhysicsShape(PhysicsEntity *owner_entity, const std::vector<Vector2Int> &vertices)
        : PhysicsShape(owner_entity), vertices(vertices) {
    }

    /**
     * @return the raw stored vertices values, not transformed in any way. To get the world position of the vertices,
     * see getVertices
     */
    [[nodiscard]] std::vector<Vector2Int> getRawVertices() const { return vertices; }
    /**
     * @return the world position of the vertices, rotated and translated based on the entity's actual position
     */
    [[nodiscard]] std::vector<Vector2Int> getVertices() const;

    PhysicsUpdateVisitor *createVisitor() override {
        return new PhysicsUpdateVisitorConvex(this);
    }

    void consumeVisitor(PhysicsUpdateVisitor *visitor, SpaceShip *space_ship) override {
        visitor->visitConvex(this, space_ship);
    }

    BoundingBox<int> getBoundingBox() override;

    bool is_inside(Vector2Int world_position) override;
};
