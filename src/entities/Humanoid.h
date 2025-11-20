#pragma once

#include "../behavior/Behavior.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "physics/PhysicsEntity.h"
#include "physics/RoundPhysicsShape.h"

class Humanoid : public PhysicsEntity{
private:
    SDL_Texture * texture;

protected:
    float radius;

public:
    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior, SDL_Texture * humanoid_texture = nullptr) :
        PhysicsEntity(position,angle,behavior,new RoundPhysicsShape(this, Scaling::scaleToWorld(20.0f)))
    {
        radius = 20;
        texture = humanoid_texture;
    }

    void update(const UpdateContext & context) override;

    void handleEvent(const SDL_Event & event) override;

    void render(SDL_Renderer *renderer, const RenderingContext & context) override;
    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override;

    void movePosition(Vector2Float screen_space_delta, SpaceShip* space_ship) override;
};
