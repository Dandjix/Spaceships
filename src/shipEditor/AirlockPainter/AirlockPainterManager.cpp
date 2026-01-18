//
// Created by timon on 12/19/25.
//


#include "AirlockPainterManager.h"

#include "spaceships/Hooks/HookRegion.h"
#include "spaceships/Hooks/airlock/Airlock.h"


void AirlockPainterManager::renderRegion(SDL_Renderer *renderer, const RenderingContext &context) const {
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

void AirlockPainterManager::update(const UpdateContext &context) {
    if (!enabled)
        return;

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

void AirlockPainterManager::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    Entity::handleEvent(event, context);

    if (!enabled) {
        return;
    }

    if (context.mouse_position_type != GameEvent::Game)
        return;
}

void AirlockPainterManager::enable() {
    enabled = true;
}

void AirlockPainterManager::disable() {
    enabled = false;
    closest_region = nullptr;
    closest_name = "";
}
