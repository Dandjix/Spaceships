//
// Created by timon on 11/21/25.
//

#include "HookRegion.h"

#include "game/Update.h"
#include "spaceships/SpaceShip.h"
void HookRegion::registerEntity(Entity* entity)
{
    Events::Id id = entity->on_destroyed.subscribe([this,entity]{
        unRegisterEntity(entity);
    });
    entities_inside.insert({entity,id});
    on_enter.emit(entity);
}

void HookRegion::unRegisterEntity(Entity* entity)
{
    auto id = entities_inside[entity];
    entities_inside.erase(entity);
    entity->on_destroyed.unsubscribe(id);
    on_leave.emit(entity);
}

bool HookRegion::pointIsInside(Vector2Int world_position) const
{
    return (
        world_position.x >= TL.x && world_position.y >= TL.y
        && world_position.x <= TL.x + dimensions.x && world_position.y <= TL.y + dimensions.y
    );
}

bool HookRegion::hasEntity(Entity* entity) const
{
    return entities_inside.contains(entity);
}

void HookRegion::update(const UpdateContext &context) {
    auto entities = context.world_instance->getEntities();

    for (auto entity: entities) {
        if (pointIsInside(entity->getPosition())) {
            if (!hasEntity(entity)) {
                registerEntity(entity);
            }
        }
        else {
            if (hasEntity(entity)) {
                unRegisterEntity(entity);
            }
        }
    }
}

void HookRegion::debugRender(SDL_Renderer *renderer, const RenderingContext &context) {

    Vector2Float TL_screen, BR_screen, TR_screen, BL_screen;

    TL_screen = context.camera_info.worldToScreenPoint(TL);
    BR_screen = context.camera_info.worldToScreenPoint(TL + dimensions);
    TR_screen = context.camera_info.worldToScreenPoint(Vector2Int(TL.x + dimensions.x,TL.y));
    BL_screen = context.camera_info.worldToScreenPoint(Vector2Int(TL.x,TL.y + dimensions.y));

    std::vector<Vector2Float> corners = {
        TL_screen,TR_screen,BR_screen,BL_screen
    };

    SDL_SetRenderDrawColor(renderer,0,255,255,255);

    for (int i = 0; i < corners.size(); ++i) {
        int next_i = (i + 1) % corners.size();

        SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[ next_i].y);
    }

}
