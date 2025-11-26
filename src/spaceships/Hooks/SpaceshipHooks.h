//
// Created by timon on 11/21/25.
//

#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include "json.hpp"
#include "json.hpp"
#include "json.hpp"
#include "behavior/Behavior.h"


class HookPoint;
class HookRegion;
struct UpdateContext;

class SpaceshipHooks
{
protected:
    std::unordered_map<std::string,HookRegion *> regions;
    std::unordered_map<std::string,HookPoint *> points;

public:
    SpaceshipHooks(
        std::unordered_map<std::string,HookRegion *> regions,
        std::unordered_map<std::string,HookPoint *> points)
    :
    regions(std::move(regions)),points(std::move(points)) {  }

    HookRegion * getRegion(const std::string& key);

    HookPoint * getPoint(const std::string& key);

    void update(const UpdateContext & context);

    [[nodiscard]] nlohmann::json toJson() const;

    static SpaceshipHooks fromJson(const nlohmann::json &entry);
};




