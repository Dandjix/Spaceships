//
// Created by timon on 11/28/25.
//

#pragma once
#include "entities/CargoContainer.h"
#include "../../entities/scripts/Entity.h"
#include "entities/Humanoid.h"
#include "entities/Sphere.h"
#include "math/Vectors.h"
#include "player/Camera.h"
#include "vehicles/combatOutfit/CombatOutfit.h"


namespace EntityLoading {
    inline Entity * fromJson(nlohmann::json entry) {

        Entity * entity = nullptr;

        auto t = entry["type"];

        if (t == nullptr)
            throw std::invalid_argument("type entry not filled");

        if (t == "humanoid") {
            entity = Humanoid::fromJson(entry);
        }
        else if (t == "sphere") {
            entity = Sphere::fromJson(entry);
        }
        else if (t == "cargo_container") {
            entity = CargoContainer::fromJson(entry);
        }
        else if (t =="camera") {
            entity = Camera::fromJson(entry);
        }
        else if (t=="combat_outfit") {
            entity = CombatOutfit::fromJson(entry);
        }
        else {
            std::string message = "t : ";
            message.append(t);
            message.append(" not handled");

            throw std::invalid_argument(message);
        }
        return entity;
    }
};
