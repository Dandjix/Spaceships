//
// Created by timon on 2/2/26.
//

#include "ConvexPhysicsShape.h"

#include <format>

#include "game/Rendering.h"
#include "physics/PhysicsUtil/BoundingBoxes.h"
#include "physics/PhysicsUtil/PhysicsUtil.h"
#include "physics/scripts/PhysicsEntity.h"

std::vector<Vector2Int> ConvexPhysicsShape::getVertices() const {
    std::vector<Vector2Int> world_vertices = {};
    world_vertices.reserve(getRawVertices().size());

    for (auto raw_vertex: getRawVertices())
        world_vertices.push_back(
            owner_entity->getPosition() + raw_vertex.rotate(owner_entity->getAngle())
        );

    return world_vertices;
}

//https://www.geeksforgeeks.org/dsa/orientation-3-ordered-points/
bool is_winding_clockwise(std::vector<Vector2Int> v) {
    if (v.size() < 3)
        return false;

    int x1 = v[0].x;
    int y1 = v[0].y;
    int x2 = v[1].x;
    int y2 = v[1].y;
    int x3 = v[2].x;
    int y3 = v[2].y;

    int val = (
                  (y2 - y1) * (x3 - x2)) - ((x2 - x1) * (y3 - y2)
              );

    return val > 0;
}

ConvexPhysicsShape::ConvexPhysicsShape(
    PhysicsEntity *owner_entity,
    const std::vector<Vector2Int> &vertices)
    : PhysicsShape(owner_entity),
      vertices(vertices) {
    //ensuring winding is always clockwise
    if (!is_winding_clockwise(this->vertices))
        std::reverse(this->vertices.begin(), this->vertices.end());
}

BoundingBox<int> ConvexPhysicsShape::getBoundingBox() const {
    return Physics::Util::createConvexBoundingBox(getVertices());
}

std::vector<float> P0CrossProducts(const std::vector<Vector2Int> & p, Vector2Int Q) {
    std::vector<float> cross_products;
    cross_products.reserve(p.size()-1);

    Vector2Int p_0 = p[0];
    for (int i = 1; i < p.size(); ++i) {
        float product = (p[i] - p_0).cross(Q - p_0);
        cross_products.push_back(product);
    }

    return cross_products;
}

unsigned long getSector(const std::vector<float> & cross_products) {
    // Check if all are non-negative (point is beyond sector 0)
    bool all_non_negative = true;
    for (auto cp : cross_products) {
        if (cp < 0) {
            all_non_negative = false;
            break;
        }
    }
    if (all_non_negative && cross_products[0] > 0) {
        return cross_products.size(); // Invalid - beyond sector 0
    }

    // Find the last negative cross product
    for (int i = cross_products.size() - 1; i >= 0; --i) {
        if (cross_products[i] < 0) {
            return i;
        }
    }

    return 0;
}

bool ConvexPhysicsShape::is_inside(Vector2Int Q) const {

    //std::cout << "computing for point Q : "<< Q<<std::endl;

    if (getRawVertices().size() < 3)
        return false;

    auto world_vertices = getVertices();

    int zero_index = 0;
    {
        int min_x = std::numeric_limits<int>::max();
        int max_y = std::numeric_limits<int>::min();

        for (int i = 0; i < world_vertices.size(); i++) {
            auto v = world_vertices[i];
            if (v.x <= min_x && v.y >= max_y) {
                min_x = v.x;
                max_y = v.y;
                zero_index = i;
            }
        }
    }

    std::ranges::rotate(world_vertices, world_vertices.begin() + zero_index);

    std::string vertices_string= "[";
    for (auto v: world_vertices) {
        vertices_string += std::format("({},{}),",v.x,v.y);
    }

    //std::cout << "points after rotation: " << vertices_string << std::endl;

    auto cross_products = P0CrossProducts(world_vertices,Q);

    // this doesnt look like it is needed
    //if first cross product is positive and last negative, we are between P1 and Pn-1 : outside
    // if (cross_products[0] > 0 && cross_products[cross_products.size()-1] < 0) {
    //     return false;
    // }

    auto sector_id = getSector(cross_products);
    if (sector_id >= cross_products.size() -1) {
        //std::cout << "sector is outside all bounds" << std::endl;
        return false;
    }
    //std::cout << "in sector : "<< sector_id << std::endl;


    // Check if point is inside the triangle formed by p_0, vertices[sector_id+1], vertices[sector_id+2]
    Vector2Int p_1 = world_vertices[sector_id + 1];
    Vector2Int p_2 = world_vertices[sector_id + 2];
    // Check if Q is on the correct side of edge p_1 -> p_2
    float cross = (p_2 - p_1).cross(Q - p_1);
    return cross <= 0;
}

void ConvexPhysicsShape::debugRender(SDL_Renderer *renderer, const RenderingContext &context) {
    PhysicsShape::debugRender(renderer, context);

    SDL_SetRenderDrawColor(renderer,255,0,0,255);

    auto points = getVertices();
    for (int i = 0; i < points.size(); ++i) {
        int next_i = (i+1)%points.size();
        Vector2Int first = points[i];
        Vector2Int second = points[next_i];

        auto first_screen = context.camera_info.worldToScreenPoint(first);
        auto second_screen = context.camera_info.worldToScreenPoint(second);

        SDL_RenderLine(renderer,first_screen.x,first_screen.y,second_screen.x,second_screen.y);
    }
}
