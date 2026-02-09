//
// Created by timon on 1/19/26.
//

#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

#include "math/Vectors.h"


class PhysicsEntity;
struct RenderingContext;
class PhysicsShape;

class HashProximityMap {
private:
    std::unordered_map<Vector2Int,std::vector<PhysicsShape *>> proximityMap;

public:
    HashProximityMap() = default;

    std::unordered_map<Vector2Int,std::vector<PhysicsShape *>>::const_iterator begin() const {return proximityMap.begin();}
    std::unordered_map<Vector2Int,std::vector<PhysicsShape *>>::const_iterator end() const {return proximityMap.end();}

    std::vector<PhysicsShape *> at(Vector2Int world_position) const;

    void render(SDL_Renderer *renderer, const RenderingContext &context) const;

    void populate(const std::vector<PhysicsEntity *> &entities);

    void clear();
};


