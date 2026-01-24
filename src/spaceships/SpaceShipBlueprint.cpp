#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "SpaceShipBlueprint.h"
#include "../math/Vectors.h"
#include "Tile.h"
#include "entities/scripts/Entity.h"
#include "EntityData/EntityLoading.h"
#include "loadGame/GameState.h"


nlohmann::json entitiesToJson(const std::vector<Entity *> & entities) {
	nlohmann::json json =  nlohmann::json::array();
	for (auto e: entities) {
		if (e->isJsonSerializable()) {
			json.push_back(e->toJson());
		}
	}
	return json;
}
std::vector<Entity *> entitiesFromJson(const nlohmann::json & json) {
	std::vector<Entity *> entities = {};
	GameState::transientGameState transient_game_state = {};
	for (const auto& e: json) {
		entities.push_back(EntityLoading::fromJson(e,transient_game_state));
	}
	for (auto e: entities) {
		e->finalizeJsonDeserialization(transient_game_state);
	}
	return entities;
}


std::string SpaceShipBlueprint::dumps() const
{
	nlohmann::json json;
	json["pathToExterior"] = pathToExterior;
	json["tiles"] = tiles;
	json["hooks"] = hooks.toJson();
	json["entities"] = entitiesToJson(entities);
	return json.dump(4);
}

SpaceShipBlueprint *SpaceShipBlueprint::loads(const std::string &from, std::filesystem::path path)
{
	nlohmann::json dict = nlohmann::json::parse(from);
	std::string pathToExterior = dict["pathToExterior"];
	std::vector<std::vector<Tile>> tiles = dict["tiles"];
	SpaceshipHooks hooks = SpaceshipHooks::fromJson(dict["hooks"]);
	std::vector<Entity*> entities = entitiesFromJson(dict["entities"]);

	return new SpaceShipBlueprint{ std::move(path), pathToExterior, tiles, entities,hooks };
}

SpaceShipBlueprint *SpaceShipBlueprint::load(const std::filesystem::path &path)
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

