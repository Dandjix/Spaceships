//
// Created by timon on 11/28/25.
//

#include "EntityPlacement.h"

#include "entities/CargoContainer.h"
#include "entities/Humanoid.h"
#include "../../../entities/toggleableActivators/Button.h"
#include "entities/toggleables/Lamp.h"

[[nodiscard]] std::unordered_map<std::string, EntityPlacement::EntitySpawner>
EntityPlacement::Registry::populateSpawners() {
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
    registry.insert(
        {
            "lamp",
            {
                [](Vector2Int position, float angle) {
                    bool on = true; //TODO : find a way to ask the user what this should be
                    return new Lamp(position, angle, EntityId::Manager::getInstance().createEntityId(), on);
                }
            }
        }
    );
    registry.insert(
        {
            "button",
            {
                [](Vector2Int position, float angle) {
                    return new Button(position, angle, nullptr);
                }
            }
        }
    );

    return registry;
}

std::vector<std::string> EntityPlacement::Registry::getOptions() {
    std::vector<std::string> options = {};
    for (const std::string &key: spawners | std::views::keys) {
        options.push_back(key);
    }
    return options;
}
