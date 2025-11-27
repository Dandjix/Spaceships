#include "BlueprintTilePainter.h"
#include <SDL3/SDL.h>
#include "../../math/Vectors.h"
#include "../../spaceships/Tile.h"

BlueprintTilePainter::BlueprintTilePainter(SpaceShipBlueprint* blueprint, ShipBuildingGrid* grid, Tile tileToPaint, bool fill)
: Entity(Vector2Int(0, 0), std::nullopt, nullptr),
blueprint(blueprint), grid(grid), tileToPaint(tileToPaint), fill(fill)
{

}

void BlueprintTilePainter::setTileToPaint(Tile tileToPaint)
{
	this->tileToPaint = tileToPaint;
}

bool BlueprintTilePainter::getEnabled()
{
	return enabled;
}

void BlueprintTilePainter::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

void BlueprintTilePainter::paintSingleTile(Vector2Int coords)
{
	blueprint->paint(coords.x, coords.y, tileToPaint);
}

void fillAndPaintRecursive(
	std::unordered_set<Vector2Int> & visited,
	Vector2Int coords,
	Tile value_to_paint,
	Tile medium,
	SpaceShipBlueprint * blueprint_to_paint)
{
	Tile value_here = blueprint_to_paint->tiles[coords.x][coords.y];

	if (visited.contains(coords) or medium!=value_here)
		return;

	blueprint_to_paint->paint(coords.x,coords.y,value_to_paint);

	visited.emplace(coords);

	int max_x = static_cast<int>(blueprint_to_paint->tiles.size())-1;
	int max_y;
	if (static_cast<int>(blueprint_to_paint->tiles.size())>0)
	{
		max_y = static_cast<int>(blueprint_to_paint->tiles[0].size())-1;
	}
	else
	{
		max_y = 0;
	}


	if (coords.x-1 >= 0)
	{
		fillAndPaintRecursive(visited,{coords.x-1,coords.y},value_to_paint,medium,blueprint_to_paint);
	}
	if (coords.x+1 <= max_x)
	{
		fillAndPaintRecursive(visited,{coords.x+1,coords.y},value_to_paint,medium,blueprint_to_paint);
	}
	if (coords.y-1 >= 0)
	{
		fillAndPaintRecursive(visited,{coords.x,coords.y-1},value_to_paint,medium,blueprint_to_paint);
	}
	if (coords.y+1 <= max_y)
	{
		fillAndPaintRecursive(visited,{coords.x,coords.y+1},value_to_paint,medium,blueprint_to_paint);
	}
}

void BlueprintTilePainter::paintFill(Vector2Int coords)
{
	std::unordered_set<Vector2Int> visitedPositions = {};

	Tile medium = blueprint->tiles[coords.x][coords.y];

	if (medium == tileToPaint)
	{
		return;
	}

	fillAndPaintRecursive(visitedPositions,coords,tileToPaint,medium,blueprint);
}


void BlueprintTilePainter::update(const UpdateContext& context)
{
	if (context.mouse_position_type != GameEvent::Game) {
		return;
	}

	if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK)
	{
		Vector2Int coords = grid->getMouseCoordinates(context.camera_info);
		if (coords == Vector2Int(-1, -1))
			return;

		if (! fill)
		{
			paintSingleTile(coords);
		}
		else
		{
			paintFill(coords);
		}
	}
}


void BlueprintTilePainter::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	Vector2Int coords = grid->getMouseCoordinates(context.camera_info);
	if (coords == Vector2Int(-1, -1))
		return;

	int pxGridSize = grid->getSizePx();

	coords = coords * Vectors::getFactor() * pxGridSize;

	Vector2Float toPaintPos = context.camera_info.worldToScreenPoint(coords);

	SDL_FRect rect = { 
		static_cast<float>(toPaintPos.x),
		static_cast<float>(toPaintPos.y),
		static_cast<float>(pxGridSize / context.camera_info.cameraScale),
		static_cast<float>(pxGridSize / context.camera_info.cameraScale)
	};
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	SDL_RenderRect(renderer, &rect);
}

void BlueprintTilePainter::setBlueprint(SpaceShipBlueprint* blueprint)
{
	this->blueprint = blueprint;
}

