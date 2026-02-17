//
// Created by timon on 2/9/26.
//

#include "ConvexCast.h"

#include "debug/CollisionInfo.h"
#include "math/BoundingBox.h"
#include "PhysicsUtil/BoundingBoxes.h"
#include "PhysicsUtil/PhysicsUtil.h"
#include "shapes/PhysicsShape.h"
#include "spaceships/SpaceShip.h"

std::vector<PhysicsShape *> Physics::ConvexCast(std::vector<Vector2Int> points, Instances::Instance *instance) {
    BoundingBox<int> cast_AABB = Physics::Util::createConvexBoundingBox(points);

    std::unordered_set<PhysicsShape *> shapes;
    for (Vector2Int cell: cast_AABB.encompassedTiles()) {
        for (PhysicsShape *shape: instance->hash_proximity_map.at_cell(cell)) {
            shapes.insert(shape);
        }
    }

    std::vector<PhysicsShape *> colliding;
    auto cast_info = Util::PolygonInfo(
        points
    );
    for (PhysicsShape *shape: shapes) {
        auto shape_info = Util::PolygonInfo(shape->getVertices());
        if (Physics::Util::areCollidingSAT(&shape_info, &cast_info)) {
            colliding.push_back(shape);
        }
    }
    return colliding;
}

std::vector<PhysicsShape *>
Physics::RectCast(
    Vector2Int rect_center,
    Vector2Int rect_dimensions,
    float angle,
    Instances::Instance *instance) {

    std::vector<Vector2Int> vertices = {
        {-rect_dimensions.x/2,-rect_dimensions.y/2},{rect_dimensions.x/2,-rect_dimensions.y/2},
        {rect_dimensions.x/2,rect_dimensions.y/2},{-rect_dimensions.x/2,rect_dimensions.y/2}
    };

    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i] = vertices[i].rotate(angle);
        vertices[i] += rect_center;
    }
    for (int i = 0; i < vertices.size(); ++i) {
        Debug::CollisionInfo::instance->addLine(vertices[i],vertices[(i+1)%vertices.size()]);
    }

    return ConvexCast(vertices,instance);
}
