#pragma once
#include <SDL3/SDL_render.h>

#include "game/Rendering.h"
#include "math/Vectors.h"

class SpaceShip;

class ParallaxObject
{
private :
    Vector2Int position;
    float angle;
    float depth;
    float sizeMultiplier;
    SDL_Texture * texture;

public:
    ParallaxObject(Vector2Int objectPosition,float objectAngle, float objectDepth, SDL_Texture * objectTexture, float objectSizeMultiplier = 1.0f)
    :
    position(objectPosition),angle(objectAngle),depth(objectDepth),sizeMultiplier(objectSizeMultiplier),texture(objectTexture){}

    void render(SDL_Renderer *renderer, const ExteriorRenderingContext &context, SpaceShip *ship) const;

    bool operator<(const ParallaxObject & other) const
    {
        return other.depth < this->depth;
    }
    bool operator<=(const ParallaxObject & other) const
    {
        return other.depth <= this->depth;
    }

    bool operator>(const ParallaxObject & other) const
    {
        return other.depth > this->depth;
    }

    bool operator>=(const ParallaxObject & other) const
    {
        return other.depth >= this->depth;
    }
};



//
// Created by timon on 11/13/25.
//
