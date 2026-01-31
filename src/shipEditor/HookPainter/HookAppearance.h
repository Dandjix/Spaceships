//
// Created by timon on 11/27/25.
//

#pragma once
#include "../../entities/scripts/ShortLivedEntity.h"


class SpaceshipHooks;

class HookAppearance : public ShortLivedEntity {
private:
    SpaceshipHooks * hooks;

    public:
    explicit HookAppearance(SpaceshipHooks * hooks)
        : ShortLivedEntity({0,0},0), hooks(hooks) {

    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    HookAppearance * initializeRendering(const EntityRendering::Context &context) override {return this;}

    HookAppearance * finalizeRendering(const EntityRendering::Context &context) override {return this;}
};
