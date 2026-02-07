#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
enum class Tile
{
	Void,
	Floor,
	Wall,
	DoorH,
	DoorV
};



class Tiles
{
private:
	static const std::unordered_map<Tile, std::string> filenames;
	static const std::unordered_map<Tile, std::string> nameMap;

	static std::unordered_map<Tile,SDL_Texture*> manifest;

	static std::string filename(Tile tile);


public:
	static constexpr int tileSizePx = 64;

	static void loadAll(SDL_Renderer* renderer);

	static void loadTileTexture(SDL_Renderer * renderer, Tile tile);

	static void unloadTileTexture(SDL_Renderer* renderer, Tile tile);

	static Tile tileFromName(const std::string & name);
	static std::string nameFromTile(Tile tile);

	static bool isTextureLoaded(Tile tile);

	static SDL_Texture* getTexture(Tile tile);
};