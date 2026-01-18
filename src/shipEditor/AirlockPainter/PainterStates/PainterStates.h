#pragma once
#include <SDL3/SDL_render.h>

#include "game/Rendering.h"

namespace AirlockPainter {
    class Painter;

    struct PainterState {
    virtual ~PainterState() = default;

    virtual void onEnter(AirlockPainter::Painter&) {}
    virtual void onExit(AirlockPainter::Painter&) {}

    virtual void render(AirlockPainter::Painter&, SDL_Renderer*, const RenderingContext&) = 0;
    virtual void handleClick(AirlockPainter::Painter&, Vector2Int world_pos) = 0;
};






// ---------------------------
// OFF
// ---------------------------
struct StateOff : PainterState {
    void render(AirlockPainter::Painter&, SDL_Renderer*, const RenderingContext&) override {}
    void handleClick(AirlockPainter::Painter&, Vector2Int) override {}
};

// ---------------------------
// AIRLOCK — dragging to form a rectangle
// ---------------------------
struct StateAirlockSelecting : PainterState {

    void render(AirlockPainter::Painter& painter,
                SDL_Renderer* r, const RenderingContext& ctx) override;

    void handleClick(AirlockPainter::Painter& painter, Vector2Int world_pos) override;
};


// ---------------------------
// AIRLOCK — waiting for first click
// ---------------------------
struct StateAirlockIdle : PainterState {

    void render(AirlockPainter::Painter& painter,
                SDL_Renderer* r, const RenderingContext& ctx) override;

    void handleClick(AirlockPainter::Painter& painter, Vector2Int world_pos) override;
};
}
