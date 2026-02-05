//
// Created by timon on 11/20/25.
//

#include "PhysicsShape.h"

#include "../PhysicsEntity.h"
#include "game/Rendering.h"


Vector2Int PhysicsShape::getCenter() const {
    return owner_entity->getPosition();
}

void PhysicsShape::physicsUpdate(const PhysicsUpdateContext& context)
{

}

void PhysicsShape::drawBoundingBox(SDL_Renderer* renderer, const RenderingContext& context)
{
    auto bbox = getBoundingBox();

    auto TL_world = bbox.topLeft();
    Vector2Int TR_world = {bbox.bottomRight().x,bbox.topLeft().y};
    auto BR_world = bbox.bottomRight();
    Vector2Int BL_world = {bbox.topLeft().x,bbox.bottomRight().y};

    Vector2Float TL = context.camera_info.worldToScreenPoint(TL_world);
    Vector2Float BR = context.camera_info.worldToScreenPoint(BR_world);
    Vector2Float TR = context.camera_info.worldToScreenPoint(TR_world);
    Vector2Float BL = context.camera_info.worldToScreenPoint(BL_world);


    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderLine(renderer,TL.x,TL.y,TR.x,TR.y);
    SDL_RenderLine(renderer,TR.x,TR.y,BR.x,BR.y);
    SDL_RenderLine(renderer,BR.x,BR.y,BL.x,BL.y);
    SDL_RenderLine(renderer,BL.x,BL.y,TL.x,TL.y);
}

void PhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    drawBoundingBox(renderer, context);
}
