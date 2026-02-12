#pragma once
#include <SDL3/SDL_render.h>

#include "math/Vectors.h"
struct RenderingContext;
//
// Created by timon on 2/12/26.
//
namespace Rendering {
    namespace Util {
        /**
         *
         * @param renderer the renderer
         * @param context the context (used mainly for the camera transformations)
         * @param center the center of the texture in the world
         * @param angle the angle of the texture, often the angle of the entity it belongs to
         * @param texture the SDL_Texture
         * @param destSize_pixels the destination size of the texture in world units
         */
        void renderTexture(
            SDL_Renderer *renderer,
            const RenderingContext &context,
            Vector2Int center,
            float angle, SDL_Texture *texture, Vector2Float destSize_pixels
        );
    }
}
