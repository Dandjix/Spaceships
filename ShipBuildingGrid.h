#pragma once
#include "Entity.h"
#include "Rendering.h"
#include <SDL3/SDL.h>
#include "Camera.h"

class ShipBuildingGrid : public Entity
{
protected :
	int sizePx;
	Vector2Int dimensions;
	bool resizing = false;
	Camera * camera;

	void renderFixed(SDL_Renderer* renderer, const RenderingContext& context)
	{
		int hLength = sizePx * dimensions.x * Vectors::getFactor();
		int vLength = sizePx * dimensions.y * Vectors::getFactor();
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int x = 0; x <= dimensions.x; x++)
		{
			int x_coord = sizePx * x * Vectors::getFactor();
			Vector2Int start(x_coord, 0);
			Vector2Int end(x_coord, vLength);
			Vector2Float startS, endS;
			startS = context.toScreenPoint(start);
			endS = context.toScreenPoint(end);
			SDL_RenderLine(renderer, startS.x, startS.y, endS.x, endS.y);
		}
		for (int y = 0; y <= dimensions.y; y++)
		{
			int y_coord = sizePx * y * Vectors::getFactor();
			Vector2Int start(0, y_coord);
			Vector2Int end(hLength, y_coord);
			Vector2Float startS, endS;
			startS = context.toScreenPoint(start);
			endS = context.toScreenPoint(end);
			SDL_RenderLine(renderer, startS.x, startS.y, endS.x, endS.y);
		}
	}


public:
	ShipBuildingGrid(int sizePx, Camera * camera) :Entity(Vector2Int(0, 0), 0.0f),sizePx(sizePx),dimensions(Vector2Int(8,16)), camera(camera) {}

	void render(SDL_Renderer* renderer, const RenderingContext& context) override {
		renderFixed(renderer, context);
	}

	void startResizing()
	{
		resizing = true;
	}

	void setDimensions(Vector2Int dimensions)
	{
		this->dimensions = dimensions;
	}

	void update(const UpdateContext & context) override
	{
		if (resizing)
		{
			float mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
		}
	}
};