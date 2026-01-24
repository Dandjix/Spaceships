//
// Created by timon on 11/28/25.
//

#include "EntityPlacement.h"

#include "entities/CargoContainer.h"
#include "entities/Humanoid.h"

[[nodiscard]]std::unordered_map<std::string, EntityPlacement::EntitySpawner> EntityPlacement::Registry::populateSpawners() {
    std::unordered_map<std::string, EntitySpawner> registry = {};


    registry.insert(
        {
            "cargo_container",
            {
                [](Vector2Int position, float angle) {
                    return new CargoContainer(position, angle);
                }
            }
        }
    );

    registry.insert({
        "humanoid",
        {
            [](Vector2Int position, float angle) {
                return new Humanoid(position, angle, nullptr);
            }
        }
    });

    return registry;
}

std::vector<std::string> EntityPlacement::Registry::getOptions() {
    std::vector<std::string> options = {};
    for (const std::string &key: spawners | std::views::keys) {
        options.push_back(key);
    }
    return options;
}
