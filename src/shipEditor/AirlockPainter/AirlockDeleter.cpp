//
// Created by timon on 11/28/25.
//

#include "AirlockDeleter.h"

#include "game/Rendering.h"
#include "spaceships/Hooks/SpaceshipHooks.h"
#include "spaceships/Hooks/airlock/Airlock.h"

void AirlockDeleter::AirlockDeleter::renderAirlock(SDL_Renderer *renderer, const RenderingContext &context) const {
    std::vector corners = {
        context.camera_info.worldToScreenPoint(closest_airlock->TL),
        context.camera_info.worldToScreenPoint({closest_airlock->TL.x + closest_airlock->dimensions.x,closest_airlock->TL.y}),
        context.camera_info.worldToScreenPoint(closest_airlock->TL + closest_airlock->dimensions),
        context.camera_info.worldToScreenPoint({closest_airlock->TL.x,closest_airlock->TL.y + closest_airlock->dimensions.y}),
    };
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    for (int i = 0; i < corners.size(); ++i) {
        auto next_i = (i+1)%corners.size();
        SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
    }
}

void AirlockDeleter::AirlockDeleter::deleteAirlock(const std::string &airlock_name) {
    hooks->removeAirlock(airlock_name);
    closest_airlock = nullptr;
    closest_name = "";
}

void AirlockDeleter::AirlockDeleter::update(const UpdateContext &context) {
    if (!enabled)
        return;

    Airlock * working_closest_airlock = nullptr;
    std::string working_closest_airlock_name;

    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);

    Vector2Int world_mouse_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});

    for (const auto& [name,airlock]: hooks->airlocks) {
        if (airlock->pointIsInside(world_mouse_point)) {
            working_closest_airlock = airlock;
            working_closest_airlock_name = name;
        }
    }

    closest_airlock = working_closest_airlock;
    closest_name = working_closest_airlock_name;
}

void AirlockDeleter::AirlockDeleter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    Entity::handleEvent(event, context);

    if (!enabled) {
        return;
    }

    if (context.mouse_position_type != GameEvent::Game)
        return;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        deleteAirlock(closest_name);

}

void AirlockDeleter::AirlockDeleter::enable() {
    enabled = true;
}

void AirlockDeleter::AirlockDeleter::disable() {
    enabled = false;
    closest_airlock = nullptr;
    closest_name = "";
}
