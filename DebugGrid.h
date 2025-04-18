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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

        Vector2Int cameraScreenPosition = Vectors::toScreenPosition(context.cameraPos);

        // Calculate top-left corner of the visible area in world units
        float startX = cameraScreenPosition.x - context.screenDimensions.x / 2;
        float startY = cameraScreenPosition.y - context.screenDimensions.y / 2;

        // Round down to nearest grid line
        float firstLineX = std::floor(startX / step) * step;
        float firstLineY = std::floor(startY / step) * step;

        // Draw vertical lines
        for (float x = firstLineX; x < startX + context.screenDimensions.x; x += step) {
            float screenX = ((x - startX));
            SDL_RenderLine(renderer, screenX, 0, screenX, static_cast<float>(context.screenDimensions.y));
        }

        // Draw horizontal lines
        for (float y = firstLineY; y < startY + context.screenDimensions.y; y += step) {
            float screenY = ((y - startY));
            SDL_RenderLine(renderer, 0, screenY, static_cast<float>(context.screenDimensions.x),screenY);
        }
    }

};