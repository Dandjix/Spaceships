//
// Created by timon on 1/18/26.
//

#pragma once
#include <SDL3/SDL_render.h>

#include "math/Vectors.h"
#include "spaceships/Hooks/airlock/Airlock.h"


struct RenderingContext;

namespace HookDebugRendering {
    void displayArrow(SDL_Renderer * renderer, const RenderingContext & context,Vector2Int worldPosition, Airlock::Orientation orientation);

    void pointPreview(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int snappedMousePosition);

    void drawAirlockPreview(SDL_Renderer *renderer, const RenderingContext &context, Vector2Int start, Vector2Int end);
};