//
// Created by timon on 11/21/25.
//

#pragma once

#include "entities/scripts/Entity.h"
#include "math/Vectors.h"
#include "events/Event.h"

class Entity;

class Airlock
{
public:
    enum Orientation {
        North,
        South,
        East,
        West
    };


    void registerEntity(Entity * entity);
    void unRegisterEntity(Entity * entity);

    Vector2Int TL;
    Orientation orientation;
    Vector2Int dimensions;
    Event<Entity*> on_enter;
    Event<Entity*> on_leave;

    std::unordered_map<Entity *,u_int64_t> entities_inside;


    Airlock(Vector2Int TL, Vector2Int dimensions,Orientation orientation) :
    TL(TL),
    dimensions(dimensions),
    orientation(orientation)
    {}

    bool pointIsInside(Vector2Int world_position) const;

    bool hasEntity(Entity * entity) const;

    void update(const UpdateContext & context);

    void debugRender(SDL_Renderer * renderer, const RenderingContext & context);

    nlohmann::json toJson() const;

    static Airlock * fromJson(const nlohmann::json &json);
};
