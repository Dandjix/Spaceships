//
// Created by timon on 11/29/25.
//

#include "GameState.h"

#include <fstream>

#include "json.hpp"
#include "behavior/BehavioredEntity.h"
#include "entities/entityId/IdentityId.h"
#include "spaceships/SpaceShip.h"


nlohmann::json gameStateToJSON(const GameState::GameState &game_state) {
    nlohmann::json json = {};

    auto json_ships = nlohmann::json::array();
    for (auto space_ship: game_state.space_ships) {
        json_ships.push_back(space_ship->toJson());
    }
    json["spaceships"] = json_ships;

    return json;
}

GameState::GameState gameStateFromJSON(const nlohmann::json json) {
    nlohmann::json json_ships = json["spaceships"];

    std::vector<SpaceShip *> ships = {};

    GameState::transientGameState transient_game_state = {};

    for (const auto &ship_entry: json_ships) {
        auto ship = SpaceShip::fromJson(ship_entry, transient_game_state);
        ships.push_back(ship);
    }

    for (auto ship: ships) {
        for (auto entity: ship->entities) {
            entity->finalizeJsonDeserialization(transient_game_state);
        }
    }

    //this part determines the max entityId

    //this part makes it so that entities placed in the editor with an id of 0 get a real entity id.
    for (const auto & [key,entity]: transient_game_state.identified_entities) {
        entity->makeReal();
    }

    return GameState::GameState(
        ships
    );
}

Camera *GameState::GameState::getCamera() {
    for (auto s: space_ships) {
        if (s->cameras.size() == 1)
            return s->cameras.at(0);
    }
    return nullptr;
}

BehavioredEntity *GameState::GameState::getPlayer() {
    for (auto space_ship: space_ships) {
        for (auto entity: space_ship->getBehavioredEntities(RoomDistance::All)) {
            if (entity->is_player()) {
                return entity;
            }
        }
    }
    return nullptr;
}

SpaceShip * GameState::GameState::getPlayerSpaceship() {
    auto player = getPlayer();

    for (auto space_ship: space_ships) {
        if (space_ship->has_entity(player))
            return space_ship;
    }
    return nullptr;
}

void GameState::dumpGameState(const GameState &game_state, const std::filesystem::path &path) {
    std::string content = dumpsGameState(game_state);

    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing: " + path.string());
    }

    out << content;
    out.close();
}

std::string GameState::dumpsGameState(GameState game_state) {
    nlohmann::json json = gameStateToJSON(game_state);
    return to_string(json);
}


GameState::GameState GameState::loadGameState(const std::filesystem::path &path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open file for reading: " + path.string());
    }

    std::stringstream buffer;
    buffer << in.rdbuf();


    return loadsGameState(buffer.str());
}

GameState::GameState GameState::loadsGameState(const std::string &content) {
    auto json = nlohmann::json::parse(content);
    auto game_state = gameStateFromJSON(json);
    return game_state;
}
