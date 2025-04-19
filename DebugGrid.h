#pragma once
#include "Entity.h"
#include "Vectors.h"
#include "Rendering.h"

class DebugGrid : public Entity {
	protected :
		float step;

	public :
	DebugGrid(Vector2Int position, float angle, float step) 
		: Entity(position,angle), step(step) {}

    void update(float deltaTime) override {
        // CargoContainer might not need to move, so just leave it empty.
    }

    void render(SDL_Renderer* renderer, const RenderingContext& context) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White grid

        float cameraScale = context.cameraScale;
        float cameraAngle = -context.cameraAngle;

        Vector2Float camPos = Vectors::toVector2Float(Vector2Int::toScreenPosition(context.cameraPos));
        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2.0f;
        Vector2Float bounds;
        if (context.screenDimensions.x > context.screenDimensions.y)
        {
            bounds = Vector2Float(context.screenDimensions.x, context.screenDimensions.x);
        }
        else
        {
            bounds = Vector2Float(context.screenDimensions.y, context.screenDimensions.y);
        }
        // Determine visible world area (rough bounds)
        Vector2Float screenSize = bounds * cameraScale;
        Vector2Float topLeftWorld = camPos - screenSize / 2.0f;
        Vector2Float bottomRightWorld = camPos + screenSize / 2.0f;

        // Align to grid step
        float startX = std::floor(topLeftWorld.x / step) * step;
        float endX = std::ceil(bottomRightWorld.x / step) * step;
        float startY = std::floor(topLeftWorld.y / step) * step;
        float endY = std::ceil(bottomRightWorld.y / step) * step;

        // Draw vertical grid lines
        for (float x = startX; x <= endX; x += step) {
            Vector2Float worldStart(x, startY);
            Vector2Float worldEnd(x, endY);

            // Transform to screen space
            Vector2Float screenStart = (worldStart - camPos) / cameraScale;
            Vector2Float screenEnd = (worldEnd - camPos) / cameraScale;

            screenStart = screenStart.rotate(-cameraAngle);
            screenEnd = screenEnd.rotate(-cameraAngle);

            screenStart = screenStart + screenCenter;
            screenEnd = screenEnd + screenCenter;

            SDL_RenderLine(renderer, screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);
        }

        // Draw horizontal grid lines
        for (float y = startY; y <= endY; y += step) {
            Vector2Float worldStart(startX, y);
            Vector2Float worldEnd(endX, y);

            Vector2Float screenStart = (worldStart - camPos) / cameraScale;
            Vector2Float screenEnd = (worldEnd - camPos) / cameraScale;

            screenStart = screenStart.rotate(-cameraAngle);
            screenEnd = screenEnd.rotate(-cameraAngle);

            screenStart = screenStart + screenCenter;
            screenEnd = screenEnd + screenCenter;

            SDL_RenderLine(renderer, screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);
        }
    }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        Vector2Int zero = Vector2Int(0, 0) - Vector2Int::toScreenPosition(context.cameraPos)/context.cameraScale; //scale zero

        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2; //rotate zero
        Vector2Float diff = (screenCenter - Vectors::toVector2Float(zero)).rotate(context.cameraAngle);
        zero = Vectors::toVector2Int( screenCenter - diff );


        DebugRendering::drawCross(renderer, zero);
    }
};