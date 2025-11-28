
#include "DebugGrid.h"

#include "../math/Vectors.h"

#include <iostream>

DebugGrid::DebugGrid(int xOrigin, int yOrigin, int grid_step) : Entity(Vector2Int(0,0),0)
{
    origin = {xOrigin, yOrigin};
    step = grid_step;
}

DebugGrid::~DebugGrid()
= default;


Vector2Int closest_point(Vector2Int position, int resolution)
{
    int closest_x = position.x % (resolution * Vectors::getFactor());
    int closest_y = position.y % (resolution * Vectors::getFactor());

    return {closest_x, closest_y};
}



Vector2Int DebugGrid::snap(Vector2Int worldPosition, bool ceil) const
{
        Vector2Int result = worldPosition.scaleToScreenPosition();
        int x, y;
        if (ceil)
        {
            x = static_cast<int>(ceilf(static_cast<float>(result.x) / step)) * step;
            y = static_cast<int>(ceilf(static_cast<float>(result.y) / step)) * step;
            return {x, y};
        }
        x = static_cast<int>(roundf(static_cast<float>(result.x) / step)) * step;
        y = static_cast<int>(roundf(static_cast<float>(result.y) / step)) * step;
        return {x, y};
    }

void DebugGrid::debugRender(SDL_Renderer* renderer, const RenderingContext& context){
        Vector2Int center = context.camera_info.cameraPosition + context.camera_info.screenDimensions.scaleToWorldPosition()*0.5f*context.camera_info.cameraScale;

        int diameter;
        if (context.camera_info.screenDimensions.y > context.camera_info.screenDimensions.x)
            diameter = context.camera_info.screenDimensions.y*2;
        else
            diameter = context.camera_info.screenDimensions.x * 2;

        diameter *= static_cast<int>(ceilf(context.camera_info.cameraScale*1.42f * Vectors::getFactor()));

        Vector2Int topLeft = snap(Vector2Int(center.x - diameter, center.y - diameter));
        Vector2Int bottomRight = snap(Vector2Int(center.x + diameter, center.y + diameter),true);

        //std::cout << "tl : " << topLeft.x << " : " << topLeft.y << "\n";
        //std::cout << "br : " << bottomRight.x << " : " << bottomRight.y << "\n";

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int x = topLeft.x; x <= bottomRight.x; x+=step)
        {
            Vector2Float pos1, pos2;
            pos1 = context.camera_info.worldToScreenPoint(Vector2Int(x, topLeft.y).scaleToWorldPosition());
            pos2 = context.camera_info.worldToScreenPoint(Vector2Int(x, bottomRight.y).scaleToWorldPosition());

            SDL_RenderLine(renderer, pos1.x,pos1.y,pos2.x,pos2.y);
        }
        for (int y = topLeft.y; y <= bottomRight.y; y += step)
        {
            Vector2Float pos1, pos2;
            pos1 = context.camera_info.worldToScreenPoint(Vector2Int(topLeft.x, y).scaleToWorldPosition());
            pos2 = context.camera_info.worldToScreenPoint(Vector2Int(bottomRight.x, y).scaleToWorldPosition());


            SDL_RenderLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
        }

    }