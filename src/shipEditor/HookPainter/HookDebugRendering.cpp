//
// Created by timon on 1/18/26.
//

#include "HookDebugRendering.h"


void HookDebugRendering::displayArrow(SDL_Renderer * renderer, const RenderingContext & context,Vector2Int worldPosition, Airlock::Orientation orientation) {
    std::vector<Vector2Float> points = {
        {-0.5,1},
        {-0.5,-0},
        {-1,-0},
        {0,-1},
        {1,-0},
        {0.5,-0},
        {0.5,1}
    };
    float angle = 0;
    switch (orientation) {
        case Airlock::North:
            angle = 0;
            break;
        case Airlock::South:
            angle = 180;
            break;
        case Airlock::East:
            angle = 90;
            break;
        case Airlock::West:
            angle = 270;
            break;
    }

    for (int i = 0; i < points.size(); i++) {
        int next_i = (i + 1)%points.size();

        Vector2Int p1_world, p2_world;
        p1_world = worldPosition + Vectors::toVector2Int(((points[i] * 48 )* Vectors::getFactor()).rotate(angle));
        p2_world = worldPosition + Vectors::toVector2Int(((points[next_i] * 48 )* Vectors::getFactor()).rotate(angle));

        Vector2Float p1_screen, p2_screen;
        p1_screen = context.camera_info.worldToScreenPoint(p1_world);
        p2_screen = context.camera_info.worldToScreenPoint(p2_world);

        SDL_RenderLine(renderer,p1_screen.x,p1_screen.y,p2_screen.x,p2_screen.y);
    }

}

void HookDebugRendering::pointPreview(SDL_Renderer *renderer, const RenderingContext &context,
    Vector2Int snappedMousePosition) {
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

void HookDebugRendering::drawAirlockPreview(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int start, Vector2Int end) {
    std::vector<Vector2Float> mouse_pos_corners = {
        context.camera_info.worldToScreenPoint(start),
        context.camera_info.worldToScreenPoint({start.x,end.y}),
        context.camera_info.worldToScreenPoint(end),
        context.camera_info.worldToScreenPoint({end.x, start.y})

    };
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for (int i = 0; i < mouse_pos_corners.size(); ++i) {
        int next_i = (i+1)%mouse_pos_corners.size();

        SDL_RenderLine(renderer,mouse_pos_corners[i].x,mouse_pos_corners[i].y,mouse_pos_corners[next_i].x,mouse_pos_corners[next_i].y);
    }
}
