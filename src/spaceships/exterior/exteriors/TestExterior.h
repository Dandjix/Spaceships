//
// Created by timon on 12/12/25.
//

#pragma once
#include "loadGame/LoadSavedGame.h"
#include "spaceships/exterior/SpaceShipExterior.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

namespace SpaceShipResources {
    class TestExterior : public Exterior{
    private:
        static SDL_Texture * albedo;
    public:
        explicit TestExterior(const Vector2Int &dimensions)
            : Exterior(dimensions) {
        }

        static void loadAssets(SDL_Renderer * renderer);

        void render(SDL_Renderer *renderer, const ExteriorRenderingContext &context, const SpaceShip *space_ship) override;
    };

}
