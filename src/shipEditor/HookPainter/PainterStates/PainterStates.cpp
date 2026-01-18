//
// Created by timon on 11/27/25.
//

#include "PainterStates.h"
#include "Painter.h"
#include "shipEditor/HookPainter/HookDebugRendering.h"




void HookPainter::StateRegionsSelecting::render(HookPainter::Painter &painter, SDL_Renderer *r,
    const RenderingContext &ctx) {
    float mx,my; SDL_GetMouseState(&mx,&my);
    auto mouse_world_pos = ctx.camera_info.screenToWorldPoint({mx,my});
    Vector2Int end = painter.snapPosition(mouse_world_pos);

    auto start = painter.getStart();

    // draw rectangle preview
    HookDebugRendering::drawAirlockPreview(r, ctx, start, end);
}

void HookPainter::StateRegionsSelecting::handleClick(HookPainter::Painter &painter, Vector2Int world_pos) {
    painter.setEndPosition(world_pos);

    painter.promptForName();   // user will call confirmPlacement()
}

void HookPainter::StatePoint::render(HookPainter::Painter &painter, SDL_Renderer *r, const RenderingContext &ctx) {
    float mx,my; SDL_GetMouseState(&mx,&my);
    auto world = ctx.camera_info.screenToWorldPoint({mx,my});
    Vector2Int snapped = painter.snapPosition(world);

    HookDebugRendering::pointPreview(r, ctx, snapped);
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
    HookDebugRendering::pointPreview(r, ctx, snapped);
}

void HookPainter::StateRegionsIdle::handleClick(HookPainter::Painter &painter, Vector2Int world_pos) {
    painter.setStart(world_pos);
    painter.changeState(std::make_unique<StateRegionsSelecting>());
}
