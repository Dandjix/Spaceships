//
// Created by timon on 11/29/25.
//

#include "GameState.h"

#include <fstream>

#include "json.hpp"
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

    for (const auto& ship_entry: json_ships) {
        auto ship = SpaceShip::fromJson(ship_entry);
        ships.push_back(ship);
    }

    return GameState::GameState(
        ships
    );
}

Camera * GameState::GameState::getCamera() {
    for (auto s: space_ships) {
        if (s->cameras.size()==1)
            return s->cameras.at(0);
    }
    return nullptr;
}

void GameState::dumpGameState(const GameState &game_state, const std::filesystem::path &path)
{
    std::string content = to_string(gameStateToJSON(game_state));

    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing: " + path.string());
    }

    out << content;
    out.close();
}

std::string GameState::dumpsGameState(const GameState &game_state) {
    return to_string(gameStateToJSON(game_state));
}


GameState::GameState GameState::loadGameState(const std::filesystem::path &path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open file for reading: " + path.string());
    }

    std::stringstream buffer;
    buffer << in.rdbuf();

    // Parse JSON (assuming nlohmann::json)
    auto json = nlohmann::json::parse(buffer.str());

    return gameStateFromJSON(json);
}

GameState::GameState GameState::loadsGameState(const std::string &content) {
    auto json = nlohmann::json::parse(content);
    return gameStateFromJSON(json);
}
