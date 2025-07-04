#include "Tile.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <stdexcept>

const std::unordered_map<Tile, std::string> Tiles::filenames{
	//{ Tile::Void,  "void" },
	{ Tile::Wall,  "wall" },
	{ Tile::DoorH, "doorh" },
	{ Tile::DoorV, "doorv" },
	{ Tile::Floor, "floor" }
};

const std::unordered_map<Tile, std::string> Tiles::nameMap{
	{Tile::Void,"Void"},
	{Tile::Wall,"Wall"},
	{Tile::DoorH,"DoorH"},
	{Tile::DoorV,"DoorV"},
	{Tile::Floor,"Floor"}
};

std::unordered_map<Tile, SDL_Texture*> Tiles::manifest;

std::string Tiles::filename(Tile tile)
{
	return filenames.at(tile) + ".png";
}



void Tiles::loadAll(SDL_Renderer* renderer)
{
	for (const auto& [tile, name] : filenames)
	{
		loadTileTexture(renderer, tile);
	}
}



void Tiles::loadTileTexture(SDL_Renderer * renderer, Tile tile)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, (std::string("assets/textures/tiles/") + filename(tile)).c_str());

	manifest.emplace(tile, texture);
}

void Tiles::unloadTileTexture(SDL_Renderer* renderer, Tile tile)
{
	SDL_Texture* texture = manifest.at(tile);
	manifest.erase(tile);
	SDL_DestroyTexture(texture);
}

Tile Tiles::tileFromName(const std::string& name)
{
	// Iterate over map to find the corresponding Tile for the name
	for (const auto& pair : nameMap) {
		if (pair.second == name) {
			return pair.first;
		}
	}

	// If not found, throw exception
	throw std::invalid_argument("Unknown tile name: " + name);
}

std::string Tiles::nameFromTile(Tile tile)
{
	return nameMap.at(tile);
}

bool Tiles::isTextureLoaded(Tile tile)
{
	return manifest.contains(tile);
}

SDL_Texture* Tiles::getTexture(Tile tile)
{
	return manifest.at(tile);
}

