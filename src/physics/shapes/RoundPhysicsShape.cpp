//
// Created by timon on 11/21/25.
//

#include "RoundPhysicsShape.h"

#include "game/Rendering.h"
#include "physics/PhysicsUtil/BoundingBoxes.h"
#include "physics/PhysicsUtil/PhysicsUtil.h"

#define CIRCLE_NB_SIDES 12

void RoundPhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    PhysicsShape::debugRender(renderer, context);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);

    //render the shape as it is generated
    auto vertices =getVertices();
    for (int i = 0; i < vertices.size(); ++i) {
        int next_i = (i+1)%vertices.size();

        Vector2Float start = context.camera_info.worldToScreenPoint(vertices[i]);
        Vector2Float end = context.camera_info.worldToScreenPoint(vertices[next_i]);
        SDL_RenderLine(renderer,start.x,start.y,end.x,end.y);
    }
}

BoundingBox<int> RoundPhysicsShape::getBoundingBox() const
{
    return Physics::Util::createRoundBoundingBox(getCenter(),radius);
}

bool RoundPhysicsShape::is_inside(Vector2Int world_position) const {
    return (owner_entity->getPosition() - world_position).sqrLength() <= radius*radius;
}

std::vector<Vector2Int> RoundPhysicsShape::getVertices() const {
    std::vector<Vector2Int> vertices;
    vertices.reserve(CIRCLE_NB_SIDES);

    for (int i = 0; i < CIRCLE_NB_SIDES; ++i) {
        float angle_radians = (static_cast<float>(i)/static_cast<float>(CIRCLE_NB_SIDES))* static_cast<float>(M_PI)*2.0f;
        Vector2Float direction = {std::cos(angle_radians),std::sin(angle_radians)};
        vertices.push_back(getCenter() +Vectors::toVector2Int( direction*radius));
    }

    return vertices;
}
