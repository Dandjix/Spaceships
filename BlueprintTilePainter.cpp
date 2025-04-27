#include "BlueprintTilePainter.h"
#include <SDL3/SDL.h>
#include "Vectors.h"
#include "Tile.h"

BlueprintTilePainter::BlueprintTilePainter(SpaceShipBlueprint* blueprint, ShipBuildingGrid* grid, Tile tileToPaint) : Entity(Vector2Int(0, 0), std::nullopt), blueprint(blueprint), grid(grid), tileToPaint(tileToPaint)
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

void BlueprintTilePainter::update(const UpdateContext& context)
{

}

void BlueprintTilePainter::handleEvent(const SDL_Event event)
{
	if (enabled && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		Vector2Int coords = grid->getMouseCoordinates();
		if (coords == Vector2Int(-1, -1))
			return;

		blueprint->paint(coords.x, coords.y, tileToPaint);
	}
}


void BlueprintTilePainter::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	Vector2Int coords = grid->getMouseCoordinates();
	if (coords == Vector2Int(-1, -1))
		return;

	int pxGridSize = grid->getSizePx();

	coords = coords * Vectors::getFactor() * pxGridSize;

	Vector2Float toPaintPos = context.toScreenPoint(coords);

	SDL_FRect rect = { 
		static_cast<float>(toPaintPos.x),
		static_cast<float>(toPaintPos.y),
		static_cast<float>(pxGridSize / context.cameraScale),
		static_cast<float>(pxGridSize / context.cameraScale)
	};
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	SDL_RenderRect(renderer, &rect);
}

void BlueprintTilePainter::setBlueprint(SpaceShipBlueprint* blueprint)
{
	this->blueprint = blueprint;
}

