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

        // Convert screen size to world units
        float worldWidth = screenWidth / factor;
        float worldHeight = screenHeight / factor;

        // Calculate top-left corner of the visible area in world units
        float startX = cameraPos.x - worldWidth / 2;
        float startY = cameraPos.y - worldHeight / 2;

        // Round down to nearest grid line
        float firstLineX = std::floor(startX / step) * step;
        float firstLineY = std::floor(startY / step) * step;

        // Draw vertical lines
        for (float x = firstLineX; x < startX + worldWidth; x += step) {
            int screenX = static_cast<int>((x - startX) * factor);
            SDL_RenderLine(renderer, static_cast<int>(screenX), 0, static_cast<int>(screenX), static_cast<int>(screenHeight));
        }

        // Draw horizontal lines
        for (float y = firstLineY; y < startY + worldHeight; y += step) {
            int screenY = static_cast<int>((y - startY) * factor);
            SDL_RenderLine(renderer, 0, static_cast<int>(screenY), screenWidth, static_cast<int>(screenY));
        }
    }

};