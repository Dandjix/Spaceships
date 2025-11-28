#pragma once

#include "../behavior/Behavior.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "behavior/BehavioredEntity.h"
#include "physics/PhysicsEntity.h"
#include "physics/RoundPhysicsShape.h"

class Humanoid : public BehavioredEntity{
private:
    SDL_Texture * texture;

protected:
    float radius;

public:
    Humanoid(Vector2Int position,std::optional<float> angle, Behavior * behavior, SDL_Texture * humanoid_texture = nullptr) :
        BehavioredEntity(position,angle,new RoundPhysicsShape(this, Scaling::scaleToWorld(20.0f)),behavior)
    {
        radius = 20;
        texture = humanoid_texture;
    }
    void render(SDL_Renderer *renderer, const RenderingContext & context) override;
};
