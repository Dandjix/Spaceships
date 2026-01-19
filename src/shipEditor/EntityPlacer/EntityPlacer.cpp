//
// Created by timon on 11/28/25.
//

#include "EntityPlacer.h"

#include "gameEvent/GameEvent.h"

void EntityPlacement::EntityPlacer::placeEntity(Vector2Int world_position, float angle, std::string to_place_key) {
    auto spawner = registry.spawners.at(to_place_key);
}

inline void EntityPlacement::EntityPlacer::update(const UpdateContext &context) {

    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
}

void EntityPlacement::EntityPlacer::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        angle_to_place_at += event.wheel.y * 10.0f;
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        placeEntity(context.camera_info.screenToWorldPoint({mouse_x,mouse_y}),angle_to_place_at,to_place);
    }
}

void EntityPlacement::EntityPlacer::render(SDL_Renderer *renderer, const RenderingContext &context) {
}
