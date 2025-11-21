//
// Created by timon on 11/21/25.
//

#include "RectPhysicsShape.h"

void RectPhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    DebugRendering::drawRect(renderer,context,dimensions,owner_entity->getPosition(),owner_entity->getAngle());
}

BoundingBox<int> RectPhysicsShape::getBoundingBox()
{
    // TODO : implement this when implementing rects
}
