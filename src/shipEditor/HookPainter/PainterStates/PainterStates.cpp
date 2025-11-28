//
// Created by timon on 11/27/25.
//

#include "PainterStates.h"
#include "Painter.h"

void pointPreview(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int snappedMousePosition) {
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

void drawRegionPreview(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int start, Vector2Int end) {
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

void HookPainter::StateRegionsSelecting::render(HookPainter::Painter &painter, SDL_Renderer *r,
    const RenderingContext &ctx) {
    float mx,my; SDL_GetMouseState(&mx,&my);
    auto mouse_world_pos = ctx.camera_info.screenToWorldPoint({mx,my});
    Vector2Int end = painter.snapPosition(mouse_world_pos);

    auto start = painter.getStart();

    // draw rectangle preview
    drawRegionPreview(r, ctx, start, end);
}

void HookPainter::StateRegionsSelecting::handleClick(HookPainter::Painter &painter, Vector2Int world_pos) {
    painter.setEndPosition(world_pos);

    painter.promptForName();   // user will call confirmPlacement()
}

void HookPainter::StatePoint::render(HookPainter::Painter &painter, SDL_Renderer *r, const RenderingContext &ctx) {
    float mx,my; SDL_GetMouseState(&mx,&my);
    auto world = ctx.camera_info.screenToWorldPoint({mx,my});
    Vector2Int snapped = painter.snapPosition(world);

    pointPreview(r, ctx, snapped);
}

void HookPainter::StatePoint::handleClick(HookPainter::Painter &painter, Vector2Int world_pos) {
    painter.setStart(world_pos);
    painter.promptForName();
}

void HookPainter::StateRegionsIdle::
render(HookPainter::Painter &painter, SDL_Renderer *r, const RenderingContext &ctx) {
    // show preview point
    float mx, my;
    SDL_GetMouseState(&mx,&my);
    auto world = ctx.camera_info.screenToWorldPoint({mx,my});

    Vector2Int snapped = painter.snapPosition(world);
    // Reuse your point-drawing logic
    // painter.renderPointHelper(...) - move that logic into a reusable fn
    pointPreview(r, ctx, snapped);
}

void HookPainter::StateRegionsIdle::handleClick(HookPainter::Painter &painter, Vector2Int world_pos) {
    painter.setStart(world_pos);
    painter.changeState(std::make_unique<StateRegionsSelecting>());
}
