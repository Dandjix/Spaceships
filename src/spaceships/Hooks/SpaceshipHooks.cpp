//
// Created by timon on 11/21/25.
//

#include "SpaceshipHooks.h"

#include "HookPoint.h"
#include "HookRegion.h"
#include "json.hpp"
#include "json.hpp"
#include "game/Update.h"
#include "spaceships/SpaceShip.h"

SpaceshipHooks::~SpaceshipHooks() {
    // for (auto val: regions | std::views::values) {
    //     delete val;
    // }
    // for (auto val: points | std::views::values) {
    //     delete val;
    // }
}

HookRegion * SpaceshipHooks::getRegion(const std::string &key) {
    if (auto it = regions.find(key); it != regions.end())
        return it->second;

    return nullptr;
}

HookPoint * SpaceshipHooks::getPoint(const std::string &key) {
    if (auto it = points.find(key); it != points.end())
        return it->second;

    return nullptr;
}

inline void SpaceshipHooks::update(const UpdateContext& context)
{
    for (auto entity : context.spaceShip->getEntities(RoomDistance::All))
    {
        for (auto entry : regions)
        {
            HookRegion * region = entry.second;
            if (region->pointIsInside(entity->getPosition()))
            {
                if (!region->hasEntity(entity))
                    region->registerEntity(entity);
            }
            else
            {
                if (region->hasEntity(entity))
                    region->unRegisterEntity(entity);
            }
        }
    }
}

[[nodiscard]]nlohmann::json SpaceshipHooks::toJson() const {
    nlohmann::json entry = {};

    {
        auto region_entries = nlohmann::json::array();
        for (const auto& r: regions) {
            auto name = r.first;
            auto region = r.second;
            region_entries.push_back({
                {"name",name},
                {"TL",region->TL.toJson()},
                {"dimensions",region->dimensions.toJson()}
            });
        }
        entry["regions"] = region_entries;
    }

    {
        auto point_entries = nlohmann::json::array();
        for (const auto& p: points) {
            auto name = p.first;
            auto point = p.second;
            point_entries.push_back({
                {"name",name},
                {"position",point->position.toJson()}
            });
        }
        entry["points"] = point_entries;
    }

    return entry;
}

SpaceshipHooks SpaceshipHooks::fromJson(const nlohmann::json &entry) {
    nlohmann::json regions = entry["regions"];
    nlohmann::json points = entry["points"];

    std::unordered_map<std::string,HookRegion *> region_map = {};

    for (auto r: regions) {
        std::string name = r["name"];
        auto TL = Vector2Int::fromJson(r["TL"]);
        auto dimensions = Vector2Int::fromJson(r["dimensions"]);
        auto region = new HookRegion(TL,dimensions);
        region_map.insert({name,region});
    }

    std::unordered_map<std::string,HookPoint *> point_map = {};

    for (auto r: points) {
        std::string name = r["name"];
        auto position = Vector2Int::fromJson(r["position"]);
        auto point = new HookPoint(position);
        point_map.insert({name,point});
    }

    SpaceshipHooks hooks = SpaceshipHooks(region_map,point_map);

    return hooks;
}
