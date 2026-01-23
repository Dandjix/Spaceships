//
// Created by timon on 11/29/25.
//

#pragma once
#include <filesystem>
#include <vector>

#include "json.hpp"
#include "entities/entityId/IdentityId.h"


class BehavioredEntity;
class Entity;
class Camera;
class SpaceShip;

namespace GameState {

    struct transientGameState {
        std::unordered_map<Entity *,nlohmann::json> transient_data_per_entity;
        std::unordered_map<EntityId::entityId,IdentifiedEntity *> identified_entities;

        transientGameState() = default;
    };


    struct GameState {
        explicit GameState(const std::vector<SpaceShip *> & space_ships) : space_ships(space_ships) {  }
        std::vector<SpaceShip * >space_ships;
        Camera * getCamera();

        BehavioredEntity *getPlayer();
        SpaceShip * getPlayerSpaceship();
    };

    void dumpGameState(const GameState & game_state, const std::filesystem::path & path);
    std::string dumpsGameState(GameState game_state);

    GameState loadGameState(const std::filesystem::path & path);
    GameState loadsGameState(const std::string & content);
}
