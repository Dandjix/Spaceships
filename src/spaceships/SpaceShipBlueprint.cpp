#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>
#include <string>

#include "SpaceShipBlueprint.h"
#include "../math/Vectors.h"
#include "Tile.h"


std::string SpaceShipBlueprint::dumps() const
{
	nlohmann::json dict;
	dict["pathToExterior"] = pathToExterior;
	dict["tiles"] = tiles;
	dict["hooks"] = hooks.toJson();
	return dict.dump(4);
}

SpaceShipBlueprint *SpaceShipBlueprint::loads(std::string from, std::filesystem::path path)
{
	nlohmann::json dict = nlohmann::json::parse(from);
	std::string pathToExterior = dict["pathToExterior"];
	std::vector<std::vector<Tile>> tiles = dict["tiles"];
	SpaceshipHooks hooks = SpaceshipHooks::fromJson(dict["hooks"]);

	return new SpaceShipBlueprint{ path, pathToExterior, tiles, hooks };
}

SpaceShipBlueprint *SpaceShipBlueprint::load(std::filesystem::path path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return loads(buffer.str(), path);
}

void SpaceShipBlueprint::resize(Vector2Int newDimensions)
{
	std::vector<std::vector<Tile>> newTiles = std::vector<std::vector<Tile>>(newDimensions.x);
	for (int x = 0; x < newDimensions.x; x++)
	{
		newTiles[x] = std::vector<Tile>(newDimensions.y);
		for (int y = 0; y < newDimensions.y; y++)
		{
			if (x < tiles.size() && y < tiles[0].size())
			{
				newTiles[x][y] = tiles[x][y];
			}
			else
			{
				newTiles[x][y] = Tile::Void;
			}
		}
	}
	tiles = newTiles;
}

void SpaceShipBlueprint::paint(int x, int y, Tile tileToPaint)
{
	tiles[x][y] = tileToPaint;
}

