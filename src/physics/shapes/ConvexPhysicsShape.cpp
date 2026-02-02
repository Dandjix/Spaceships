//
// Created by timon on 2/2/26.
//

#include "ConvexPhysicsShape.h"

#include "physics/PhysicsEntity.h"

std::vector<Vector2Int> ConvexPhysicsShape::getVertices() const {
    std::vector<Vector2Int> world_vertices = {};
    world_vertices.reserve(getRawVertices().size());

    for (auto raw_vertex: getRawVertices())
        world_vertices.push_back(
            owner_entity->getPosition() + raw_vertex.rotate(owner_entity->getAngle())
        );

    return world_vertices;
}

BoundingBox<int> ConvexPhysicsShape::getBoundingBox() {
    // Define corners in local space
    std::vector<Vector2Int> world_vertices = getVertices();

    int min_x = std::numeric_limits<int>::max(), max_x = std::numeric_limits<int>::min(), min_y = std::numeric_limits<
        int>::max(), max_y = std::numeric_limits<int>::min();

    for (auto c: world_vertices) {
        if (c.x < min_x)
            min_x = c.x;
        if (c.x > max_x)
            max_x = c.x;
        if (c.y < min_y)
            min_y = c.y;
        if (c.y > max_y)
            max_y = c.y;
    }

    return BoundingBox<int>({min_x, min_y}, {max_x - min_x, max_y - min_y});
}

bool ConvexPhysicsShape::is_inside(Vector2Int world_position) {
    //TODO : implement
    return false;
}
