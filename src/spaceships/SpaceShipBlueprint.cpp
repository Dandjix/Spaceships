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


nlohmann::json entitiesToJson(const std::vector<Entity *> &entities) {
    nlohmann::json json = nlohmann::json::array();
    for (auto e: entities) {
        if (e->isJsonSerializable()) {
            json.push_back(e->toJson());
        }
    }
    return json;
}

std::vector<Entity *> entitiesFromJson(const nlohmann::json &json,
                                       GameState::transientGameState &transient_game_state,
                                       EntityRendering::Context *entity_rendering_context) {
    std::vector<Entity *> entities = {};

    for (const auto &e: json) {
        entities.push_back(EntityLoading::fromJson(e, transient_game_state, entity_rendering_context));
    }
    return entities;
}


std::string SpaceShipBlueprint::dumps() const {
    nlohmann::json json;
    json["pathToExterior"] = pathToExterior;
    json["tiles"] = tiles;
    json["hooks"] = hooks.toJson();
    json["entities"] = entitiesToJson(entities);
    return json.dump(4);
}

SpaceShipBlueprint *SpaceShipBlueprint::loads(const std::string &from, std::filesystem::path path,
                                              GameState::transientGameState &transient_game_state,
                                              EntityId::Manager &entity_id_manager,
                                              EntityRendering::Context *entity_rendering_context,
                                              bool create_blueprint_entities,
                                              bool finalize_json_deserialization) {
    nlohmann::json json = nlohmann::json::parse(from);

    if (create_blueprint_entities) {
        entity_id_manager.incrementEntityIds(json);
    }


    std::string pathToExterior = json["pathToExterior"];
    std::vector<std::vector<Tile> > tiles = json["tiles"];
    SpaceshipHooks hooks = SpaceshipHooks::fromJson(json["hooks"]);

    std::vector<Entity *> entities;
    if (create_blueprint_entities) {
        entities = entitiesFromJson(json["entities"], transient_game_state, entity_rendering_context);

        if (finalize_json_deserialization)
            for (auto e: entities)
                e->finalizeJsonDeserialization(transient_game_state);
    } else
        entities = {};

    return new SpaceShipBlueprint{std::move(path), pathToExterior, tiles, entities, hooks};
}

SpaceShipBlueprint *SpaceShipBlueprint::load(const std::filesystem::path &path,
                                             GameState::transientGameState &transient_game_state,
                                             EntityId::Manager &entity_id_manager,
                                             EntityRendering::Context *entity_rendering_context,
                                             bool create_blueprint_entities,
                                             bool finalize_json_deserialization) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return loads(buffer.str(), path, transient_game_state, entity_id_manager, entity_rendering_context,
                 create_blueprint_entities, finalize_json_deserialization);
}

void SpaceShipBlueprint::resize(Vector2Int newDimensions) {
    std::vector<std::vector<Tile> > newTiles = std::vector<std::vector<Tile> >(newDimensions.x);
    for (int x = 0; x < newDimensions.x; x++) {
        newTiles[x] = std::vector<Tile>(newDimensions.y);
        for (int y = 0; y < newDimensions.y; y++) {
            if (x < tiles.size() && y < tiles[0].size()) {
                newTiles[x][y] = tiles[x][y];
            } else {
                newTiles[x][y] = Tile::Void;
            }
        }
    }
    tiles = newTiles;
}

void SpaceShipBlueprint::paint(int x, int y, Tile tileToPaint) {
    tiles[x][y] = tileToPaint;
}
