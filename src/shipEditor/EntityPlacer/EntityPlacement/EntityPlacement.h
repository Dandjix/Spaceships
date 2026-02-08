//
// Created by timon on 11/28/25.
//

#pragma once
#include <filesystem>
#include <functional>
#include <variant>

#include "../../../entities/scripts/Entity.h"

namespace EntityPlacement {
    struct EntitySpawner {
        std::function<Entity*(Vector2Int position, float angle)> construct;
    };

    class Registry {
    public:
        std::unordered_map<std::string, EntitySpawner> spawners;

        static std::unordered_map<std::string, EntitySpawner> populateSpawners();

        std::vector<std::string> getOptions();

        explicit Registry() : spawners(populateSpawners()) {}
    };
}
