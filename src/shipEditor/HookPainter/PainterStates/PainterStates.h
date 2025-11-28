#pragma once
#include <SDL3/SDL_render.h>

#include "game/Rendering.h"

namespace HookPainter {
    class Painter;

    struct PainterState {
    virtual ~PainterState() = default;

    virtual void onEnter(HookPainter::Painter&) {}
    virtual void onExit(HookPainter::Painter&) {}

    virtual void render(HookPainter::Painter&, SDL_Renderer*, const RenderingContext&) = 0;
    virtual void handleClick(HookPainter::Painter&, Vector2Int world_pos) = 0;
};






// ---------------------------
// OFF
// ---------------------------
struct StateOff : PainterState {
    void render(HookPainter::Painter&, SDL_Renderer*, const RenderingContext&) override {}
    void handleClick(HookPainter::Painter&, Vector2Int) override {}
};





// ---------------------------
// REGIONS — dragging to form a rectangle
// ---------------------------
struct StateRegionsSelecting : PainterState {

    void render(HookPainter::Painter& painter,
                SDL_Renderer* r, const RenderingContext& ctx) override;

    void handleClick(HookPainter::Painter& painter, Vector2Int world_pos) override;
};


// ---------------------------
// POINT PLACEMENT
// ---------------------------
struct StatePoint : PainterState {

    void render(HookPainter::Painter& painter,
                SDL_Renderer* r, const RenderingContext& ctx) override;

    void handleClick(HookPainter::Painter& painter, Vector2Int world_pos) override;
};

// ---------------------------
// REGIONS — waiting for first click
// ---------------------------
struct StateRegionsIdle : PainterState {

    void render(HookPainter::Painter& painter,
                SDL_Renderer* r, const RenderingContext& ctx) override;

    void handleClick(HookPainter::Painter& painter, Vector2Int world_pos) override;
};
}
