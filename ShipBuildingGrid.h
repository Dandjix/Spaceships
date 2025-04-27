#pragma once
#include "Entity.h"
#include "Rendering.h"
#include <SDL3/SDL.h>
#include "Camera.h"

class ShipBuildingGrid : public Entity
{
private:
	void drawLines(Vector2Int dimensions, SDL_Renderer* renderer, const RenderingContext& context) const
	{
		int hLength = sizePx * dimensions.x * Vectors::getFactor();
		int vLength = sizePx * dimensions.y * Vectors::getFactor();
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

	std::function<void(Vector2Int dimensions)> onResize;

protected :
	int sizePx;
	Vector2Int dimensions;
	bool resizing = false;
	Camera * camera;



	void renderFixed(SDL_Renderer* renderer, const RenderingContext& context)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		drawLines(dimensions, renderer, context);
	}

	void renderFluid(SDL_Renderer* renderer, const RenderingContext& context) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		Vector2Int newDimensions = getMouseCoordinates() + Vector2Int(1,1);
		drawLines(newDimensions, renderer, context);
	}

public:
	ShipBuildingGrid(int sizePx, Vector2Int dimensions, Camera * camera, std::function<void(Vector2Int dimensions)> onResize) 
		:Entity(Vector2Int(0, 0), std::nullopt)
		,sizePx(sizePx),dimensions(dimensions), resizing(false), camera(camera), onResize(onResize) {}

	void render(SDL_Renderer* renderer, const RenderingContext& context) override {
		if (!resizing)
		{
			renderFixed(renderer, context);
		}
		else
		{
			renderFluid(renderer, context);
		}
	}

	int getSizePx() const
	{
		return sizePx;
	}

	Vector2Int getMouseCoordinates() const
	{
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		Vector2Int worldCursorPoint = camera->screenToWorldPoint(Vector2Float(mouseX, mouseY));

		int factor = Vectors::getFactor() * sizePx;

		Vector2Int newDimensions = worldCursorPoint / factor;

		newDimensions = Vector2Int(newDimensions.x, newDimensions.y);

		if (newDimensions.x <= 0 || newDimensions.y <= 0)
			newDimensions = Vector2Int(0, 0);

		return newDimensions;
	}

	void startResizing()
	{
		resizing = true;
	}

	void setDimensions(Vector2Int dimensions)
	{
		this->dimensions = dimensions;
	}

	void handleEvent(const SDL_Event event) override
	{
		if (resizing)
		{
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				Vector2Int newDimensions = getMouseCoordinates() + Vector2Int(1,1);
				onResize(newDimensions);
				dimensions = newDimensions;
				resizing = false;
			}
		}
	}

	void update(const UpdateContext & context) override
	{

	}
};