//
// Created by timon on 11/21/25.
//

#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include "json.hpp"


class Airlock;
class HookPoint;
class HookRegion;
struct UpdateContext;

class SpaceshipHooks
{
public:
    std::unordered_map<std::string,HookRegion *> regions;
    std::unordered_map<std::string,HookPoint *> points;
    std::unordered_map<std::string,Airlock *> airlocks;

    SpaceshipHooks(
        std::unordered_map<std::string,HookRegion *> regions,
        std::unordered_map<std::string,HookPoint *> points,
        std::unordered_map<std::string,Airlock *> airlocks)
    :
    regions(std::move(regions)),points(std::move(points)),airlocks(std::move(airlocks)) {  }

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
    void addAirlock(const std::string & name, Airlock * airlock) {
        airlocks.insert({name,airlock});
    }


    void removePoint(const std::string &name) {
        points.erase(name);
    }


    void update(const UpdateContext & context);

    [[nodiscard]] nlohmann::json toJson() const;

    void removeAirlock(const std::string & name) {
        airlocks.erase(name);
    }


    static SpaceshipHooks * fromJson(const nlohmann::json &entry);
};




