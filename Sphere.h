#pragma once
#include "RoundEntity.h"
#include "Entity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Vectors.h"
#include "Rendering.h"

class Sphere : public RoundEntity 
{
private:
    static SDL_Texture* texture;
    static bool texturesLoaded;
public:

    Sphere(Vector2Int position, int radius) : RoundEntity (position, std::nullopt,radius)
    {

    }

    static void LoadTextures(SDL_Renderer* renderer) {
        if (texturesLoaded) return;

        texture = IMG_LoadTexture(renderer, "assets/textures/objects/spheres/1.png");

        texturesLoaded = true;
    }


    void render(SDL_Renderer* renderer, const RenderingContext& context) override {
       Vector2Float halfSize = Vector2Float(static_cast<float>(radius), static_cast<float>(radius)) / context.cameraScale;
       renderTexture(renderer, context, texture, halfSize);
    }

    void update(float deltaTime)
    {

    }
};

SDL_Texture* Sphere::texture = nullptr;
bool Sphere::texturesLoaded = false;