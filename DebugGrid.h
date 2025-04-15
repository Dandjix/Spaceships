#pragma once
#include "Entity.h"
#include "Vector2Float.h"
#include "Vector2Int.h"
#include "Vectors.h"

class DebugGrid : public Entity {
	protected :
		float step;

	public :
	DebugGrid(int x, int y, float step) 
		: Entity(x,y), step(step) {}

    void update(float deltaTime) override {
        // CargoContainer might not need to move, so just leave it empty.
    }

    void render(SDL_Renderer* renderer, const Vector2Float cameraPos, int screenWidth, int screenHeight) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

        // Calculate top-left corner of the visible area in world units
        float startX = cameraPos.x - screenWidth / 2;
        float startY = cameraPos.y - screenHeight / 2;

        // Round down to nearest grid line
        float firstLineX = std::floor(startX / step) * step;
        float firstLineY = std::floor(startY / step) * step;

        // Draw vertical lines
        for (float x = firstLineX; x < startX + screenWidth; x += step) {
            float screenX = ((x - startX));
            SDL_RenderLine(renderer, screenX, 0, screenX, static_cast<float>(screenHeight));
        }

        // Draw horizontal lines
        for (float y = firstLineY; y < startY + screenHeight; y += step) {
            float screenY = ((y - startY));
            SDL_RenderLine(renderer, 0, screenY, static_cast<float>(screenWidth),screenY);
        }
    }

};