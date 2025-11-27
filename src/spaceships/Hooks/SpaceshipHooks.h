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
public:
    std::unordered_map<std::string,HookRegion *> regions;
    std::unordered_map<std::string,HookPoint *> points;


    SpaceshipHooks(
        std::unordered_map<std::string,HookRegion *> regions,
        std::unordered_map<std::string,HookPoint *> points)
    :
    regions(std::move(regions)),points(std::move(points)) {  }

    ~SpaceshipHooks();

    HookRegion * getRegion(const std::string& key);

    HookPoint * getPoint(const std::string& key);

    void addRegion(const std::string& name, HookRegion * region) {
        regions.insert({name,region});
    }
    void removeRegion(const std::string &name) {
        regions.erase(name);
    }

    void addPoint(const std::string& name, HookPoint * point) {
        points.insert({name,point});

    }
    void removePoint(const std::string &name) {
        points.erase(name);
    }


    void update(const UpdateContext & context);

    [[nodiscard]] nlohmann::json toJson() const;

    static SpaceshipHooks fromJson(const nlohmann::json &entry);
};




