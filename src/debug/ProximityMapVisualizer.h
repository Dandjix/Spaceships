#pragma once
#include "entities/scripts/ShortLivedEntity.h"
//
// Created by timon on 2/9/26.
//

namespace Debug {
    class ProximityMapVisualizer : public ShortLivedEntity {
    protected:
        SpaceShip *space_ship;

    public:
        explicit ProximityMapVisualizer(SpaceShip *space_ship)
            : ShortLivedEntity({0, 0}, 0), space_ship(space_ship) {
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        ProximityMapVisualizer *initializeRendering(const EntityRendering::Context &context) override { return this; }

        ProximityMapVisualizer *finalizeRendering(const EntityRendering::Context &context) override { return this; }
    };
}
