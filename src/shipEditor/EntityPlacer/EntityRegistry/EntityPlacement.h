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
    std::string hook_point_prefix;
    std::function<Entity*(Vector2Int position, float angle)> construct;
};

class Registry {

public:
    std::map<std::string,EntitySpawner> spawners;

    static std::map<std::string,EntitySpawner> populateSpawners();

    explicit Registry() : spawners(populateSpawners()) {}
};
}
