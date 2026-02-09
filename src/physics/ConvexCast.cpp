//
// Created by timon on 2/9/26.
//

#include "ConvexCast.h"

#include "math/BoundingBox.h"
#include "PhysicsUtil/BoundingBoxes.h"
#include "PhysicsUtil/PhysicsUtil.h"
#include "shapes/PhysicsShape.h"
#include "spaceships/SpaceShip.h"

std::vector<PhysicsShape *> Physics::ConvexCast(std::vector<Vector2Int> points, SpaceShip *space_ship) {
    BoundingBox<int> cast_AABB = Physics::Util::createConvexBoundingBox(points);

    std::unordered_set<PhysicsShape *> shapes;
    for (Vector2Int cell: cast_AABB.encompassedTiles()) {
        for (PhysicsShape *shape: space_ship->hash_proximity_map.at_cell(cell)) {
            shapes.insert(shape);
        }
    }

    std::vector<PhysicsShape *> colliding;
    auto cast_info = Util::PolygonInfo(
        points
    );
    for (PhysicsShape *shape: shapes) {
        auto shape_info = Util::PolygonInfo(shape->getVertices());
        if (Physics::Util::areCollidingSAT(&shape_info,&cast_info)) {
            colliding.push_back(shape);
        }
    }
    return colliding;
}
