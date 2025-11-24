#include "TileRendering.h"

void TileRendering::renderTile(SDL_Renderer* renderer, const RenderingContext& context, Tile tile, int x, int y)
{
	if (tile == Tile::Void)
		return;
	SDL_Texture* texture = Tiles::getTexture(tile);

	Vector2Int worldPosition = Vector2Int(x, y) * Vectors::getFactor() * Tiles::tileSizePx;

	Vector2Float screenPos = context.camera_info.worldToScreenPoint(worldPosition);

	SDL_FRect dstrect = {
		screenPos.x,
		screenPos.y,
		Tiles::tileSizePx / context.camera_info.cameraScale,
		Tiles::tileSizePx / context.camera_info.cameraScale
	};
	SDL_FPoint topLeft = { 0.0f, 0.0f };
	SDL_RenderTextureRotated(renderer, texture, nullptr, &dstrect,context.camera_info.cameraAngle, &topLeft,SDL_FlipMode::SDL_FLIP_NONE);
}