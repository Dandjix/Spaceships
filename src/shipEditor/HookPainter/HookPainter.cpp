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

Vector2Int HookPainter::Painter::snapPosition(Vector2Int world_point) {

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
    return adjusted_world_point;
}

void HookPainter::Painter::renderRegionHelper(SDL_Renderer *renderer, const RenderingContext &context) {
    auto start_pos_value = start_position.value();

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    auto world_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});
    Vector2Int snappedMousePosition = snapPosition(world_point);

    std::vector<Vector2Float> region_corners = {
        context.camera_info.worldToScreenPoint(start_pos_value),
        context.camera_info.worldToScreenPoint({start_pos_value.x,snappedMousePosition.y}),
        context.camera_info.worldToScreenPoint(snappedMousePosition),
        context.camera_info.worldToScreenPoint({snappedMousePosition.x,start_pos_value.y})
    };
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for (int i = 0; i < region_corners.size(); ++i) {
        int next_i = (i+1)%region_corners.size();

        SDL_RenderLine(renderer,region_corners[i].x,region_corners[i].y,region_corners[next_i].x,region_corners[next_i].y);
    }
}

void HookPainter::Painter::renderPointHelper(SDL_Renderer *renderer, const RenderingContext &context) {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    auto world_point = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});
    Vector2Int snappedMousePosition = snapPosition(world_point);

    std::vector<Vector2Float> mouse_pos_corners = {
        context.camera_info.worldToScreenPoint(snappedMousePosition + Vector2Int(32,32).scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(snappedMousePosition + Vector2Int(-32,32).scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(snappedMousePosition + Vector2Int(-32,-32).scaleToWorldPosition()),
        context.camera_info.worldToScreenPoint(snappedMousePosition + Vector2Int(32,-32).scaleToWorldPosition())

    };
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for (int i = 0; i < mouse_pos_corners.size(); ++i) {
        int next_i = (i+1)%mouse_pos_corners.size();

        SDL_RenderLine(renderer,mouse_pos_corners[i].x,mouse_pos_corners[i].y,mouse_pos_corners[next_i].x,mouse_pos_corners[next_i].y);
    }
}

void HookPainter::Painter::render(SDL_Renderer *renderer, const RenderingContext &context) {
    if (state == Off)
        return;
    if (!start_position.has_value() && state == Regions) {
        renderPointHelper(renderer,context);
        return;
    }
    if (start_position.has_value() && state == Regions) {
        renderRegionHelper(renderer, context);
        return;
    }
    if (state == Point) {
        renderPointHelper(renderer,context);
        return;
    }
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
        Vector2Int adjusted_world_point = snapPosition(world_point);

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
