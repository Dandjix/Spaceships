//
// Created by timon on 11/28/25.
//

#include "PointDeleter.h"
#include "spaceships/Hooks/HookPoint.h"

void HookDeleter::PointDeleter::deletePoint(const std::string &point_name) {
    hooks->removePoint(point_name);
    closest_point = nullptr;
    closest_name = "";
}

void HookDeleter::PointDeleter::renderPoint(SDL_Renderer *renderer, const RenderingContext &context) const {
    std::vector corners = {
        context.camera_info.worldToScreenPoint(closest_point->position + Vector2Int{32,32}.scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(closest_point->position + Vector2Int{-32,32}.scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(closest_point->position + Vector2Int{-32,-32}.scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(closest_point->position + Vector2Int{32,-32}.scaleToWorldPosition()),
    };
    // auto rect =SDL_FRect(
    //     closest_point->position.x,
    //     closest_point->position.y,
    //     32,
    //     32
    // );
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    // SDL_RenderFillRect(renderer,&rect);
    for (int i = 0; i < corners.size(); ++i) {
        auto next_i = (i+1)%corners.size();
        SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
    }
}

void HookDeleter::PointDeleter::update(const UpdateContext &context) {
    HookPoint * working_closest_point = nullptr;
    std::string working_closest_point_name;
    float current_shortest_distance = std::numeric_limits<float>::max();

    float mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);

    Vector2Int world_mouse_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});

    for (const auto& [name,point]: hooks->points) {
        float distance = (point->position - world_mouse_point).length();
        if (distance < current_shortest_distance) {
            distance = current_shortest_distance;
            working_closest_point = point;
            working_closest_point_name = name;
        }
    }

    closest_point = working_closest_point;
    closest_name = working_closest_point_name;
}

void HookDeleter::PointDeleter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    Entity::handleEvent(event, context);

    if (!enabled) {
        return;
    }

    if (context.mouse_position_type != GameEvent::Game)
        return;

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        deletePoint(closest_name);
}

void HookDeleter::PointDeleter::enable() {
    enabled = true;
}

void HookDeleter::PointDeleter::disable() {
    closest_point = nullptr;
    closest_name = "";
    enabled = false;
}
