#include "ShipBuildingGrid.h"
#include "../math/Vectors.h"
#include "../spaceships/Tile.h"

void ShipBuildingGrid::drawLines(Vector2Int dimensions, SDL_Renderer* renderer, const RenderingContext& context) const
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

void ShipBuildingGrid::renderFixed(SDL_Renderer* renderer, const RenderingContext& context)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	drawLines(dimensions, renderer, context);
}

void ShipBuildingGrid::renderFluid(SDL_Renderer* renderer, const RenderingContext& context)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2Int newDimensions = getMouseCoordinates(false) + Vector2Int(1, 1);
	drawLines(newDimensions, renderer, context);
}

ShipBuildingGrid::ShipBuildingGrid(Vector2Int dimensions, Camera* camera, std::function<void(Vector2Int dimensions)> onResize) :Entity(Vector2Int(0, 0), std::nullopt)
, sizePx(Tiles::tileSizePx), dimensions(dimensions), resizing(false), camera(camera), onResize(onResize)
{

}

void ShipBuildingGrid::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	if (!resizing)
	{
		renderFixed(renderer, context);
	}
	else
	{
		renderFluid(renderer, context);
	}
}

int ShipBuildingGrid::getSizePx() const
{
	return sizePx;
}

Vector2Int ShipBuildingGrid::getMouseCoordinates(bool strict) const
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2Int worldCursorPoint = camera->screenToWorldPoint(Vector2Float(mouseX, mouseY));

	int factor = Vectors::getFactor() * sizePx;

	Vector2Int coordinates = worldCursorPoint / factor;

	coordinates = Vector2Int(coordinates.x, coordinates.y);

	if (coordinates.x < 0 || coordinates.y < 0 || (strict && (coordinates.x >= dimensions.x || coordinates.y >= dimensions.y)))
		coordinates = Vector2Int(-1, -1);

	return coordinates;
}

void ShipBuildingGrid::startResizing()
{
	resizing = true;
}

Camera* ShipBuildingGrid::getCamera()
{
	return camera;
}

void ShipBuildingGrid::setDimensions(Vector2Int dimensions)
{
	this->dimensions = dimensions;
}

void ShipBuildingGrid::handleEvent(const SDL_Event & event)
{
	if (resizing)
	{
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			Vector2Int newDimensions = getMouseCoordinates(false) + Vector2Int(1, 1);
			onResize(newDimensions);
			dimensions = newDimensions;
			resizing = false;
		}
	}
}

void ShipBuildingGrid::update(const UpdateContext& context)
{

}

