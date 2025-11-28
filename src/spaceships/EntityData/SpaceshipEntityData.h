//
// Created by timon on 11/28/25.
//

#pragma once
#include "entities/CargoContainer.h"
#include "entities/Entity.h"
#include "entities/Humanoid.h"
#include "entities/Sphere.h"
#include "math/Vectors.h"


namespace SpaceshipEntityData {
    Entity * entityFromJson(nlohmann::json entry) {

        Entity * entity;

        auto t = entry["type"];

        if (t == "humanoid") {
            entity = Humanoid::fromJson(entry);
        }
        else if (t == "sphere") {
            entity = Sphere::fromJson(entry);
        }
        else if (t == "cargo_container") {
            entity = CargoContainer::fromJson(entry);
        }
    }
};
