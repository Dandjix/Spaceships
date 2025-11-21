//
// Created by timon on 11/21/25.
//

#include "HookRegion.h"

void HookRegion::registerEntity(Entity* entity)
{
    uint64_t id = entity->on_die.subscribe([this,entity]{
        unRegisterEntity(entity);
    });
    entities_inside.insert({entity,id});
}

void HookRegion::unRegisterEntity(Entity* entity)
{
    auto id = entities_inside[entity];
    entities_inside.erase(entity);
    entity->on_die.unsubscribe(id);
}

bool HookRegion::pointIsInside(Vector2Int world_position) const
{
    return (
        world_position.x >= position.x && world_position.y >= position.y
        && world_position.x <= position.x + dimensions.x && world_position.y <= position.y + dimensions.y
    );
}

bool HookRegion::hasEntity(Entity* entity) const
{
    return entities_inside.contains(entity);
}

void HookRegion::update(const UpdateContext& context)
{
    Entity::update(context);

    auto entities = context.spaceShip->getEntities(RoomDistance::All); //TODO: only filter through the entities that are immediate to the room of this entity

    for (auto e : entities)
    {
        if (hasEntity(e) && e)
        {

        }
    }
}
