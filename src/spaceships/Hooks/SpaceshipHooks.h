//
// Created by timon on 11/21/25.
//

#pragma once
#include <string>
#include <unordered_map>

#include "HookPoint.h"
#include "HookRegion.h"


struct UpdateContext;

class SpaceshipHooks
{
protected:
    std::unordered_map<std::string,HookRegion *> regions;
    std::unordered_map<std::string,HookPoint *> points;

public:
    HookRegion * getRegion(const std::string& key)
    {
        if (auto it = regions.find(key); it != regions.end())
            return it->second;

        return nullptr;
    }

    HookPoint * getPoint(const std::string& key)
    {
        if (auto it = points.find(key); it != points.end())
            return it->second;

        return nullptr;
    }

    void update(const UpdateContext & context);
};




