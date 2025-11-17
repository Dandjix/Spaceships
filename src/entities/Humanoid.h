#pragma once

#include "RoundEntity.h"
#include "../behavior/Behavior.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Humanoid : public RoundEntity{
private:
    SDL_Texture * texture;

public:
    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior, SDL_Texture * humanoid_texture = nullptr)
        : RoundEntity(position,angle,20,behavior)
    {
        texture = humanoid_texture;
    }

    void update(const UpdateContext & context) override;

    void handleEvent(const SDL_Event & event) override;

    void render(SDL_Renderer *renderer, const RenderingContext & context) override;

    void movePosition(Vector2Float screen_space_delta, SpaceShip* space_ship) override;
};
