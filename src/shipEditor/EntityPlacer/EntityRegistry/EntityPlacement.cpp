//
// Created by timon on 11/28/25.
//

#include "EntityPlacement.h"

#include "entities/CargoContainer.h"
#include "entities/Humanoid.h"

std::map<std::string, EntityPlacement::EntitySpawner> EntityPlacement::Registry::populateSpawners() {
    std::map<std::string, EntityPlacement::EntitySpawner> registry = {};


    registry.insert(
        {
            "cargo_container",
            {
                "cargo_container_",
                [](Vector2Int position, float angle) {
                    return new CargoContainer(position, angle);
                }
            }
        }
    );

    registry.insert({
        "humanoid",
        {
            "human_",
            [](Vector2Int position, float angle) {
                return new Humanoid(position, angle, nullptr);
            }
        }
    });

    return registry;
}
