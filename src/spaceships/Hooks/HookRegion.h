//
// Created by timon on 11/21/25.
//

#pragma once
#include <memory>
#include <optional>
#include <utility>

#include "entities/Entity.h"
#include "math/Vectors.h"


class Entity;

class HookRegion : public Entity
{
public:

    void registerEntity(Entity * entity);
    void unRegisterEntity(Entity * entity);

    Vector2Int dimensions;
    std::function<void(Entity *)> on_enter;
    std::function<void(Entity *)> on_leave;
    std::unordered_map<Entity *,u_int64_t> entities_inside;


    HookRegion(Vector2Int TL, Vector2Int dimensions, std::function<void(Entity *)> on_enter = {}, std::function<void(Entity *)> on_leave = {})
        : Entity(TL, std::nullopt,nullptr),
    dimensions(dimensions),on_enter(std::move(on_enter)),on_leave(std::move(on_leave))
    {}

    bool pointIsInside(Vector2Int world_position) const;

    bool hasEntity(Entity * entity) const;

    void update(const UpdateContext & context) override;
};
