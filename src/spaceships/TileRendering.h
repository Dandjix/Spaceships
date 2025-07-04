#pragma once
#include "../game/Rendering.h"
#include "../spaceships/Tile.h"
#include <SDL3/SDL.h>
class TileRendering {
public:
	static void renderTile(SDL_Renderer* renderer, const RenderingContext& context, Tile tile, int x, int y);
};
