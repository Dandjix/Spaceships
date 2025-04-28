#include "BlueprintEditorAppearance.h"
#include "SpaceShipBlueprint.h"

void BlueprintEditorAppearance::renderTile(SDL_Renderer* renderer, const RenderingContext& context, Tile tile, int x, int y)
{
	if (tile == Tile::Void)
		return;
	SDL_Texture* texture = Tiles::getTexture(tile);

	Vector2Int worldPosition = Vector2Int(x, y) * Vectors::getFactor() * Tiles::tileSizePx;

	Vector2Float screenPos = context.toScreenPoint(worldPosition);

	SDL_FRect dstrect = {
		screenPos.x,
		screenPos.y,
		Tiles::tileSizePx / context.cameraScale,
		Tiles::tileSizePx / context.cameraScale
	};

	SDL_RenderTexture(renderer, texture, nullptr, &dstrect);
}

BlueprintEditorAppearance::BlueprintEditorAppearance(SpaceShipBlueprint* blueprint) : 
	Entity(Vector2Int(0,0),std::nullopt), 
	blueprint(blueprint){}

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

