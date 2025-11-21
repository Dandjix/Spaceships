//
// Created by timon on 11/21/25.
//

#include "RectPhysicsShape.h"

void RectPhysicsShape::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    PhysicsShape::debugRender(renderer, context);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    DebugRendering::drawRect(renderer,context,dimensions.scaleToScreenPosition(),owner_entity->getPosition(),owner_entity->getAngle());
}

BoundingBox<int> RectPhysicsShape::getBoundingBox()
{
    // Calculate half size with camera scale
    Vector2Float halfSize = dimensions * 0.5f;

    // Define corners in local space
    std::vector<Vector2Int> corners = {
        owner_entity->getPosition() + Vectors::toVector2Int(Vector2Float(-halfSize.x, -halfSize.y).rotate(owner_entity->getAngle())),
        owner_entity->getPosition() + Vectors::toVector2Int(Vector2Float( halfSize.x, -halfSize.y).rotate(owner_entity->getAngle())),
        owner_entity->getPosition() + Vectors::toVector2Int(Vector2Float( halfSize.x,  halfSize.y).rotate(owner_entity->getAngle())),
        owner_entity->getPosition() + Vectors::toVector2Int(Vector2Float(-halfSize.x,  halfSize.y).rotate(owner_entity->getAngle()))
    };
    int min_x = std::numeric_limits<int>::max(), max_x= std::numeric_limits<int>::min(), min_y= std::numeric_limits<int>::max(), max_y= std::numeric_limits<int>::min();

    for (auto c : corners)
    {
        if (c.x < min_x)
            min_x = c.x;
        if (c.x > max_x)
            max_x = c.x;
        if (c.y < min_y)
            min_y = c.y;
        if (c.y > max_y)
            max_y = c.y;
    }

    return BoundingBox<int>({min_x,min_y},{max_x-min_x,max_y-min_y});
    // TODO : implement this when implementing rects
}
