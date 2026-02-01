//
// Created by timon on 11/28/25.
//

#include "EntityLoading.h"

#include <iostream>

#include "entities/CargoContainer.h"
#include "entities/Humanoid.h"
#include "entities/Sphere.h"
#include "entities/toggleableActivators/Button.h"
#include "entities/toggleables/Lamp.h"
#include "player/Camera.h"
#include "vehicles/combatOutfit/CombatOutfit.h"
#include "vehicles/spaceship/PilotSeat.h"

Entity *EntityLoading::fromJson(nlohmann::json entry, GameState::transientGameState &transient_game_state,
                                EntityRendering::Context *entity_rendering_context) {
    Entity *entity = nullptr;

    auto t = entry["type"];

    if (t == nullptr) {
        std::cout << entry << std::endl;
        throw std::invalid_argument("type entry not filled");
    }

    if (EntityFactory::getInstance().contains(t)) {
        auto factory = EntityFactory::getInstance().at(t);
        entity = factory(entry,transient_game_state);
    } else {
        std::string type = t;
        throw std::runtime_error(std::format("fromJson not implemented properly for {}",type));
    }

    if (entity_rendering_context != nullptr)
        return entity->initializeRendering(*entity_rendering_context);

    return entity;
}
