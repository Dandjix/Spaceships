//
// Created by timon on 11/28/25.
//

#include "EntityPlacer.h"

#include "game/Rendering.h"
#include "gameEvent/GameEvent.h"

void EntityPlacement::EntityPlacer::placeEntity(Vector2Int world_position, float angle, const std::string &to_place_key) {
    auto spawner = registry->spawners.at(to_place_key);
    auto entity = spawner.construct(world_position,angle);
    if (entity_rendering_context != nullptr)
        entity->initializeRendering(*entity_rendering_context);
    placed_entities->push_back(entity);
}

void EntityPlacement::EntityPlacer::setPrecision(Precision value) {
    precision = value;
}

inline void EntityPlacement::EntityPlacer::update(const UpdateContext &context) {

    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
}

void EntityPlacement::EntityPlacer::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL && SDL_GetModState() && SDL_KMOD_ALT) {
        angle_to_place_at += event.wheel.y * 10.0f;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && context.mouse_position_type == GameEvent::Game) {
        float mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        placeEntity(context.camera_info.screenToWorldPoint({mouse_x,mouse_y}),angle_to_place_at,to_place);
    }
}

void EntityPlacement::EntityPlacer::render(SDL_Renderer *renderer, const RenderingContext &context) {
    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    Vector2Int world_position = context.camera_info.screenToWorldPoint(Vector2Float{mouse_x,mouse_y});
    Vector2Int offset_world_position = world_position + Vector2Int(Scaling::scaleToWorld(96),0).rotate(angle_to_place_at);

    Vector2Float screen_position = context.camera_info.worldToScreenPoint(world_position);
    Vector2Float offset_screen_position = context.camera_info.worldToScreenPoint(offset_world_position);

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderLine(renderer,screen_position.x,screen_position.y,offset_screen_position.x,offset_screen_position.y);
}
