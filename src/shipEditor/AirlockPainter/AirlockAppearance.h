//
// Created by timon on 12/19/25.
//

#pragma once
#include "entities/scripts/ShortLivedEntity.h"


class SpaceshipHooks;

class AirlockAppearance : public ShortLivedEntity {
private:
    SpaceshipHooks *hooks;

public:
    explicit AirlockAppearance(SpaceshipHooks *hooks)
        : ShortLivedEntity({0, 0}, 0), hooks(hooks) {
    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;
};
