//
// Created by timon on 11/21/25.
//

#pragma once
#include <memory>
#include <optional>
#include <utility>

#include "entities/Entity.h"
#include "math/Vectors.h"
#include "events/Event.h"

class Entity;

class HookRegion
{
public:

    void registerEntity(Entity * entity);
    void unRegisterEntity(Entity * entity);

    Vector2Int TL;
    Vector2Int dimensions;
    Event<Entity*> on_enter;
    Event<Entity*> on_leave;

    std::unordered_map<Entity *,u_int64_t> entities_inside;


    HookRegion(Vector2Int TL, Vector2Int dimensions) :
    TL(TL),
    dimensions(dimensions)
    {}

    bool pointIsInside(Vector2Int world_position) const;

    bool hasEntity(Entity * entity) const;

    void update(const UpdateContext & context);

    void debugRender(SDL_Renderer * renderer, const RenderingContext & context);
};
