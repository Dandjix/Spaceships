//
// Created by timon on 11/28/25.
//

#pragma once
#include "entities/CargoContainer.h"
#include "../../entities/scripts/Entity.h"
#include "entities/Humanoid.h"
#include "entities/Sphere.h"
#include "../../entities/toggleableActivators/Button.h"
#include "entities/toggleables/Lamp.h"
#include "math/Vectors.h"
#include "player/Camera.h"
#include "vehicles/combatOutfit/CombatOutfit.h"
#include "vehicles/spaceship/PilotSeat.h"


namespace GameState {
    struct transientGameState;
}

namespace EntityLoading {
    inline Entity * fromJson(nlohmann::json entry, GameState::transientGameState & transient_game_state, EntityRendering::Context * entity_rendering_context) {

        Entity * entity = nullptr;

        auto t = entry["type"];

        if (t == nullptr) {
            std::cout << entry << std::endl;
            throw std::invalid_argument("type entry not filled");
        }

        if (t == "humanoid") {
            entity = Humanoid::fromJson(entry, transient_game_state);
        }
        else if (t == "sphere") {
            entity = Sphere::fromJson(entry, transient_game_state);
        }
        else if (t == "cargo_container") {
            entity = CargoContainer::fromJson(entry, transient_game_state);
        }
        else if (t =="camera") {
            entity = Camera::fromJson(entry, transient_game_state);
        }
        else if (t=="combat_outfit") {
            entity = CombatOutfit::fromJson(entry, transient_game_state);
        }
        else if (t=="pilot_seat") {
            entity = PilotSeat::fromJson(entry, transient_game_state);
        }
        else if (t=="button") {
            entity = Button::fromJson(entry,transient_game_state);
        }
        else if (t=="lamp") {
            entity = Lamp::fromJson(entry,transient_game_state);
        }
        else {
            std::string message = "t : ";
            message.append(t);
            message.append(" not handled");

            throw std::invalid_argument(message);
        }

        if (entity_rendering_context != nullptr)
            return entity->initializeRendering(*entity_rendering_context);

        return entity;
    }
};
