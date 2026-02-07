//
// Created by timon on 11/21/25.
//

#include "RoundPhysicsShape.h"

#include "game/Rendering.h"

void RoundPhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    PhysicsShape::debugRender(renderer, context);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);

    //render the shape as it is generated
    auto vertices =generateVertices();
    for (int i = 0; i < vertices.size(); ++i) {
        int next_i = (i+1)%vertices.size();

        Vector2Float start = context.camera_info.worldToScreenPoint(vertices[i]);
        Vector2Float end = context.camera_info.worldToScreenPoint(vertices[next_i]);
        SDL_RenderLine(renderer,start.x,start.y,end.x,end.y);
    }
}

BoundingBox<int> RoundPhysicsShape::getBoundingBox() const
{
    Vector2Int TL;

    TL = owner_entity->getPosition() - Vector2Int( radius,radius);

    Vector2Int dimensions = {static_cast<int>(std::ceil(radius*2)),static_cast<int>(std::ceil(radius*2))};

    return {TL,dimensions};
}

bool RoundPhysicsShape::is_inside(Vector2Int world_position) const {
    return (owner_entity->getPosition() - world_position).sqrLength() <= radius*radius;
}

std::vector<Vector2Int> RoundPhysicsShape::generateVertices(int nb_sides) const {
    std::vector<Vector2Int> vertices;
    vertices.reserve(nb_sides);

    for (int i = 0; i < nb_sides; ++i) {
        float angle_radians = (static_cast<float>(i)/static_cast<float>(nb_sides))* M_PI*2.0f;
        Vector2Float direction = {std::cos(angle_radians),std::sin(angle_radians)};
        vertices.push_back(getCenter() +Vectors::toVector2Int( direction*radius));
    }

    return vertices;
}
