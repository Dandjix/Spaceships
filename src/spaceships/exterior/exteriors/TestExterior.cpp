//
// Created by timon on 12/12/25.
//

#include "TestExterior.h"

SDL_Texture* SpaceShipResources::TestExterior::albedo = nullptr;

void SpaceShipResources::TestExterior::loadAssets(SDL_Renderer *renderer) {
    if (albedo) {
        return;
    }
    albedo = IMG_LoadTexture(renderer,ENV_PROJECT_ROOT"assets/spaceships/test.png");
}

void SpaceShipResources::TestExterior::render(SDL_Renderer *renderer, const ExteriorRenderingContext &context, const SpaceShip *space_ship) {
    SDL_Texture* texture = albedo;

    if (!texture) {
        // Texture not found
        return;
    }

    // Calculate the half-size of the container

    // Render the texture

    {
        Vector2Float destSize = Vectors::toVector2Float(dimensions).scaleToScreenPosition();
        destSize = destSize / context.camera_info.cameraScale;

        Vector2Float center = context.camera_info.worldToScreenPoint(space_ship->getCenter());

        SDL_FRect destRect = {
            center.x - destSize.x,
            center.y - destSize.y,
            destSize.x * 2.0f,
            destSize.y * 2.0f};
        float ship_angle = context.camera_info.getScreenObjectAngle(space_ship->getAngle());

        SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, ship_angle, nullptr, SDL_FLIP_NONE);
    }
}
