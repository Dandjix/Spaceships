//
// Created by timon on 2/12/26.
//

#include "RenderTexture.h"

#include "game/Rendering.h"

void Rendering::Util::renderTexture(
    SDL_Renderer *renderer,
    const RenderingContext &context,
    Vector2Int center,
    float angle,
    SDL_Texture *texture,
    Vector2Float destSize_pixels
) {
    destSize_pixels = destSize_pixels / context.camera_info.cameraScale;

    Vector2Float screen_texture_center = context.camera_info.worldToScreenPoint(center);

    SDL_FRect destRect = {
        screen_texture_center.x - destSize_pixels.x,
        screen_texture_center.y - destSize_pixels.y,
        destSize_pixels.x * 2.0f,
        destSize_pixels.y * 2.0f
    };
    SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, angle, nullptr, SDL_FLIP_NONE);
}
