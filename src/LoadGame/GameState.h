//
// Created by timon on 11/29/25.
//

#pragma once
#include <filesystem>
#include <vector>

#include "json.hpp"


class Entity;
class Camera;
class SpaceShip;

namespace GameState {
    struct GameState {
        explicit GameState(const std::vector<SpaceShip *> & space_ships) : space_ships(space_ships) {  }
        std::vector<SpaceShip * >space_ships;
        Camera * getCamera();
        Entity * getPlayer();
        SpaceShip * getPlayerSpaceship();

        void after_deserialized(nlohmann::json json);

    };

    void dumpGameState(const GameState & game_state, const std::filesystem::path & path);
    std::string dumpsGameState(GameState game_state);

    GameState loadGameState(const std::filesystem::path & path);
    GameState loadsGameState(const std::string & content);
}
