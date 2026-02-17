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
    explicit ConvexPhysicsShape(PhysicsEntity *owner_entity, const std::vector<Vector2Int> &vertices);

    void setRawVertices(const std::vector<Vector2Int> &vertices_param) {
        vertices = vertices_param;
    }

    /**
     * @return the raw stored vertices values, not transformed in any way. To get the world position of the vertices,
     * see getVertices.
     * winding is always clockwise
     */
    [[nodiscard]] std::vector<Vector2Int> getRawVertices() const { return vertices; }
    /**
     * @return the world position of the vertices, rotated and translated based on the entity's actual position.
     * winding is always clockwise
     */
    [[nodiscard]] std::vector<Vector2Int> getVertices() const override;


    PhysicsUpdateVisitor *createVisitor() override {
        return new PhysicsUpdateVisitorConvex(this);
    }

    void consumeVisitor(PhysicsUpdateVisitor *visitor, Instances::Instance *instance) override {
        visitor->visitConvex(this, instance);
    }

    BoundingBox<int> getBoundingBox() const override;

    bool is_inside(Vector2Int world_position) const override;

    void debugRender(SDL_Renderer *renderer, const RenderingContext &context) override;
};
