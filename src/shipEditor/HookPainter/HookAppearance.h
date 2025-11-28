//
// Created by timon on 11/27/25.
//

#pragma once
#include "entities/Entity.h"


class HookAppearance : public Entity {
private:
    SpaceshipHooks * hooks;

    public:
    explicit HookAppearance(SpaceshipHooks * hooks)
        : Entity({0,0},0), hooks(hooks) {

    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;
};
