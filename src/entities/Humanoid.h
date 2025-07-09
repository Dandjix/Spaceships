#pragma once

#include "RoundEntity.h"
#include "../behavior/Behavior.h"

class Humanoid : public RoundEntity{
public:
    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior)
        : RoundEntity(position,angle,20,behavior) {}

    void update(const UpdateContext & context) override;

    void handleEvent(const SDL_Event & event) override;

    void render(SDL_Renderer *renderer, const RenderingContext & context) override;
};
