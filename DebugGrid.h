#pragma once
#include "Entity.h"
#include "Vectors.h"
#include "Rendering.h"

class DebugGrid : public Entity {
private :
    Vector2Int snap(Vector2Int worldPosition, bool ceil = false) const
    {
        Vector2Int result = worldPosition.scaleToScreenPosition();
        int x, y;
        if (ceil)
        {
            x = static_cast<int>(ceilf(static_cast<float>(result.x) / step)) * step;
            y = static_cast<int>(ceilf(static_cast<float>(result.y) / step)) * step;
            return Vector2Int(x, y);
        }
        x = static_cast<int>(roundf(static_cast<float>(result.x) / step)) * step;
        y = static_cast<int>(roundf(static_cast<float>(result.y) / step)) * step;
        return Vector2Int(x, y);
    }
protected :
	/// <summary>
	/// in screen pixels, not in world units
	/// </summary>
	int step;

public :
	DebugGrid(Vector2Int position, float angle, int step) 
		: Entity(position,angle), step(step) {}

    void update(float deltaTime) override {
        // CargoContainer might not need to move, so just leave it empty.
    }

    void render(SDL_Renderer* renderer, const RenderingContext& context) {
        Vector2Int center = context.cameraPos + context.screenDimensions.scaleToWorldPosition()*0.5f*context.cameraScale;

        int diameter;
        if (context.screenDimensions.y > context.screenDimensions.x)
            diameter = context.screenDimensions.y*2;
        else
            diameter = context.screenDimensions.x * 2;

        diameter *= static_cast<int>(ceilf(context.cameraScale*1.42f * Vectors::getFactor()));

        Vector2Int topLeft = snap(Vector2Int(center.x - diameter, center.y - diameter));
        Vector2Int bottomRight = snap(Vector2Int(center.x + diameter, center.y + diameter),true);

        //std::cout << "tl : " << topLeft.x << " : " << topLeft.y << "\n";
        //std::cout << "br : " << bottomRight.x << " : " << bottomRight.y << "\n";

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int x = topLeft.x; x <= bottomRight.x; x+=step)
        {
            Vector2Float pos1, pos2;
            pos1 = context.toScreenPoint(Vector2Int(x, topLeft.y).scaleToWorldPosition());
            pos2 = context.toScreenPoint(Vector2Int(x, bottomRight.y).scaleToWorldPosition());

            SDL_RenderLine(renderer, pos1.x,pos1.y,pos2.x,pos2.y);
        }
        for (int y = topLeft.y; y <= bottomRight.y; y += step)
        {
            Vector2Float pos1, pos2;
            pos1 = context.toScreenPoint(Vector2Int(topLeft.x, y).scaleToWorldPosition());
            pos2 = context.toScreenPoint(Vector2Int(bottomRight.x, y).scaleToWorldPosition());


            SDL_RenderLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
        }

    }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        Vector2Int zero = Rendering::get_zero(context);

        DebugRendering::drawCross(renderer, zero);
    }
};