//
// Created by timon on 11/27/25.
//

#pragma once
#include "entities/ShortLivedEntity.h"


class HookAppearance : public ShortLivedEntity {
private:
    SpaceshipHooks * hooks;

    public:
    explicit HookAppearance(SpaceshipHooks * hooks)
        : ShortLivedEntity({0,0},0), hooks(hooks) {

    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;
};
