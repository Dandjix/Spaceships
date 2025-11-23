//
// Created by timon on 11/21/25.
//

#include "RoundPhysicsShape.h"

void RoundPhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    PhysicsShape::debugRender(renderer, context);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    DebugRendering::drawCircle(renderer,context,owner_entity->getPosition(),radius);
}

BoundingBox<int> RoundPhysicsShape::getBoundingBox()
{
    Vector2Int TL;

    TL = owner_entity->getPosition() - Vector2Int( radius,radius);

    Vector2Int dimensions = {static_cast<int>(std::ceil(radius*2)),static_cast<int>(std::ceil(radius*2))};

    return BoundingBox<int>(TL,dimensions);
}
