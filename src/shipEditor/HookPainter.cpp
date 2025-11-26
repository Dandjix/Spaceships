//
// Created by timon on 11/24/25.
//

#include "HookPainter.h"

void HookPainter::Painter::setState(State new_state) {
    state = new_state;
    cancelPlacement();
}

void HookPainter::Painter::setPrecision(Precision new_precision) {
    precision = new_precision;
}

void HookPainter::Painter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    Entity::handleEvent(event, context);

    if (context.mouse_position_type != GameEvent::Game || state == Off) {
        return;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        auto world_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});
        Vector2Float world_point_pixels = Vectors::toVector2Float(world_point).scaleToScreenPosition() / Tiles::tileSizePx;
        Vector2Int adjusted_world_point;
        switch (precision) {
            case Center:
                adjusted_world_point = {
                    static_cast<int>(std::floor(world_point_pixels.x)),
                    static_cast<int>(std::floor(world_point_pixels.y))
                };
                adjusted_world_point =
                    adjusted_world_point.scaleToWorldPosition()
                    * Tiles::tileSizePx +
                    Vector2Int(Tiles::tileSizePx/2,Tiles::tileSizePx/2).scaleToWorldPosition();

                break;
            case Intersection:

                adjusted_world_point = {
                    static_cast<int>(std::round(world_point_pixels.x)),
                    static_cast<int>(std::round(world_point_pixels.y))
                };
                adjusted_world_point =
                    adjusted_world_point.scaleToWorldPosition()
                    * Tiles::tileSizePx;

                break;
            case Free:
                adjusted_world_point = world_point;
                break;
        }

        switch (state) {
            case Regions:
                if (!start_position.has_value()) {
                    startPlacing(adjusted_world_point);
                }
                else {
                    confirmPlacement(adjusted_world_point);
                }
                break;
            case Point:
                confirmPlacement(adjusted_world_point);
                break;
        }
    }
}

void HookPainter::Painter::startPlacing(Vector2Int position) {
    start_position = position;
}

void HookPainter::Painter::cancelPlacement() {
    start_position = std::nullopt;
}

void HookPainter::Painter::confirmPlacement(Vector2Int end_position) {
    if (state == Regions) {
        Vector2Int TL;
        Vector2Int start_position_value = start_position.value();

        if (start_position_value.x < end_position.x) {
            TL.x = start_position_value.x;
        }
        else {
            TL.x = end_position.x;
        }
        if (start_position_value.y < end_position.y) {
            TL.y = start_position_value.y;
        }
        else {
            TL.y = end_position.y;
        }
        Vector2Int diff = start_position_value - end_position;

        Vector2Int dimensions = {std::abs(diff.x),std::abs(diff.y)};

        auto region_name = promptForName();
        if (region_name.empty())
            return;

        start_position = std::nullopt;
        on_region_placed.emit(region_name,TL,dimensions);
        return;
    }
    if (state == Point) {
        auto point_name = promptForName();
        if (point_name.empty())
            return;

        on_point_placed.emit(point_name,end_position);
        return;
    }
}
