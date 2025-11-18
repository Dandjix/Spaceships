#pragma once

#include "RoundEntity.h"
#include "../behavior/Behavior.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "physics/PhysicsEntity.h"
#include "physics/RoundPhysicsEntity.h"

class Humanoid : public RoundPhysicsEntity{
private:
    SDL_Texture * texture;

public:
    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior, SDL_Texture * humanoid_texture = nullptr)
        : RoundPhysicsEntity(position,angle,20,behavior),
        Entity(position,angle,behavior)
    {
        texture = humanoid_texture;
    }

    void update(const UpdateContext & context) override;

    void handleEvent(const SDL_Event & event) override;

    void render(SDL_Renderer *renderer, const RenderingContext & context) override;

    void movePosition(Vector2Float screen_space_delta, SpaceShip* space_ship) override;
};
