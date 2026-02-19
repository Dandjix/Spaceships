//
// Created by timon on 11/29/25.
//

#pragma once
#include <filesystem>
#include <vector>

#include "json.hpp"
#include "entities/entityId/EntityId.h"
#include "entityRendering/RenderingInitialization.h"


class BehavioredEntity;
class Entity;
class Camera;
class SpaceShip;

namespace GameState {

    struct transientGameState {
        std::unordered_map<Entity *,nlohmann::json> transient_data_per_entity;
        std::unordered_map<EntityId::entityId,IIdentifiedEntity *> identified_entities;

        transientGameState() = default;

        [[nodiscard]] EntityId::entityId maxEntityId();
    };


    struct GameState {
        explicit GameState(const std::vector<SpaceShip *> & space_ships, EntityId::entityId next_entity_id)
        :
        space_ships(space_ships),
        next_entity_id(next_entity_id) {
        }

        std::vector<SpaceShip * >space_ships;
        EntityId::entityId next_entity_id;
        Camera * getCamera();

        BehavioredEntity *getPlayer();
        SpaceShip * getPlayerSpaceship();
    };

    void dumpGameState(const GameState & game_state, const std::filesystem::path & path);
    std::string dumpsGameState(GameState game_state);

    GameState loadGameState(const std::filesystem::path &path, EntityId::Manager &entity_id_manager, EntityRendering::Context *
                            entity_rendering_context);
    GameState loadsGameState(const std::string &content, EntityId::Manager &entity_id_manager, EntityRendering::Context *entity_rendering_context);
}
