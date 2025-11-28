//
// Created by timon on 11/28/25.
//

#include "RegionDeleter.h"

#include "spaceships/Hooks/HookPoint.h"
#include "spaceships/Hooks/HookRegion.h"

void HookDeleter::RegionDeleter::renderRegion(SDL_Renderer *renderer, const RenderingContext &context) const {
    std::vector corners = {
        context.camera_info.worldToScreenPoint(closest_region->TL),
        context.camera_info.worldToScreenPoint({closest_region->TL.x + closest_region->dimensions.x,closest_region->TL.y}),
        context.camera_info.worldToScreenPoint(closest_region->TL + closest_region->dimensions),
        context.camera_info.worldToScreenPoint({closest_region->TL.x,closest_region->TL.y + closest_region->dimensions.y}),
    };
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    for (int i = 0; i < corners.size(); ++i) {
        auto next_i = (i+1)%corners.size();
        SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
    }
}

void HookDeleter::RegionDeleter::deleteRegion(const std::string &region_name) {
    hooks->removeRegion(region_name);
    closest_region = nullptr;
    closest_name = "";
}

void HookDeleter::RegionDeleter::update(const UpdateContext &context) {
    if (!enabled)
        return;

    Entity::update(context);

    HookRegion * working_closest_region = nullptr;
    std::string working_closest_region_name;

    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);

    Vector2Int world_mouse_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});

    for (const auto& [name,region]: hooks->regions) {
        if (region->pointIsInside(world_mouse_point)) {
            working_closest_region = region;
            working_closest_region_name = name;
        }
    }

    closest_region = working_closest_region;
    closest_name = working_closest_region_name;
}

void HookDeleter::RegionDeleter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    Entity::handleEvent(event, context);

    if (!enabled) {
        return;
    }

    if (context.mouse_position_type != GameEvent::Game)
        return;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        deleteRegion(closest_name);

}

void HookDeleter::RegionDeleter::enable() {
    enabled = true;
}

void HookDeleter::RegionDeleter::disable() {
    enabled = false;
    closest_region = nullptr;
    closest_name = "";
}
