//
// Created by timon on 12/12/25.
//

#pragma once
#include <SDL3/SDL_render.h>

#include "math/Vectors.h"

struct ExteriorRenderingContext;
class SpaceShip;

namespace SpaceShipResources {

    class Exterior {
    public:
        Vector2Int dimensions;

        Exterior(Vector2Int dimensions) : dimensions(dimensions) {  }

        virtual ~Exterior() = default;

        static void loadAssets();

        virtual void render(SDL_Renderer * renderer, const ExteriorRenderingContext & context, SpaceShip * space_ship) = 0;
    };
}

