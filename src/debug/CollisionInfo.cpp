//
// Created by timon on 2/6/26.
//

#include "CollisionInfo.h"

#include "game/Rendering.h"
#include "shipEditor/HookPainter/HookDebugRendering.h"

Debug::CollisionInfo* Debug::CollisionInfo::instance = nullptr;

void Debug::CollisionInfo::render(SDL_Renderer *renderer, const RenderingContext &context) {
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    for (auto point: points) {
        DebugRendering::drawCross(renderer,context.camera_info.worldToScreenPoint(point));
    }

    for (auto line: lines) {
        Vector2Float screen_p1, screen_p2;
        screen_p1 = context.camera_info.worldToScreenPoint(line.first);
        screen_p2 = context.camera_info.worldToScreenPoint(line.second);

        DebugRendering::drawArrow(renderer,screen_p1,0);

        SDL_RenderLine(renderer,screen_p1.x,screen_p1.y,screen_p2.x,screen_p2.y);
    }
}

void Debug::CollisionInfo::addLine(Vector2Int start, Vector2Int end) {
    next_lines.push_back({start,end});
}

void Debug::CollisionInfo::addPoint(Vector2Int point) {
    next_points.push_back(point);
}

void Debug::CollisionInfo::update(const UpdateContext &context) {
    lines = next_lines;
    points = next_points;

    next_lines = {};
    next_points = {};
}
