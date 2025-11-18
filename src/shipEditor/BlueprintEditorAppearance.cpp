#include "BlueprintEditorAppearance.h"
#include "../spaceships/SpaceShipBlueprint.h"
#include "../spaceships/TileRendering.h"

void BlueprintEditorAppearance::renderTile(SDL_Renderer* renderer, const RenderingContext& context, Tile tile, int x, int y)
{
	TileRendering::renderTile(renderer, context, tile, x, y);
}

BlueprintEditorAppearance::BlueprintEditorAppearance(SpaceShipBlueprint* blueprint) : 
	Entity(Vector2Int(0,0),std::nullopt,nullptr), blueprint(blueprint){}

void BlueprintEditorAppearance::update(const UpdateContext& context){}

void BlueprintEditorAppearance::render(SDL_Renderer* renderer, const RenderingContext& context)
{
	for (int x = 0; x < blueprint->tiles.size(); x++)
	{
		std::vector<Tile> column = blueprint->tiles[x];
		for (int y = 0; y < column.size(); y++)
		{
			Tile tile = column[y];
			renderTile(renderer, context, tile, x, y);
		}
	}
}

