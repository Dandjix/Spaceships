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
        Vector2Float floatPosition = Vectors::toVector2Float(position);



        Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);
        Vector2Float worldCenter = floatPosition - floatCameraPosition;
        Vector2Float center = (worldCenter) / context.cameraScale;
        center = Vector2Float::toScreenPosition(center);

        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2;
        Vector2Float diff = (screenCenter - center).rotate(context.cameraAngle);
        //center = center - diff;
        center = screenCenter - diff;

        Vector2Float halfSize = Vector2Float(static_cast<float>(radius), static_cast<float>(radius)) / context.cameraScale;


        SDL_FRect destRect = {
            center.x - halfSize.x,
            center.y - halfSize.y,
            halfSize.x * 2.0f,
            halfSize.y * 2.0f
        };

        SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, getAngle(), nullptr, SDL_FLIP_NONE);

    /*    SDL_SetRenderDrawColorFloat(renderer, 255, 0, 0, 255);
        DebugRendering::drawCross(renderer, (Vectors::toVector2Int(center)));*/

        //Vector2Int marked = Vectors::toVector2Int(center) - Vector2Int::toScreenPosition(context.cameraPos);
        //std::cout << marked.x << ":" << marked.y << "\n";
        //SDL_SetRenderDrawColorFloat(renderer, 255, 0, 0, 255);
        //DebugRendering::drawCross(renderer, (marked));

        SDL_SetRenderDrawColorFloat(renderer, 0, 255, 0, 255);

        //DebugRendering::drawLine(renderer, Vector2Int(0, 0), Vector2Int::toScreenPosition(context.cameraPos));

        SDL_SetRenderDrawColorFloat(renderer, 0, 255, 255, 255);

        DebugRendering::drawLine(renderer, Vectors::toVector2Int(screenCenter), Vectors::toVector2Int(center));

        SDL_SetRenderDrawColorFloat(renderer, 255, 255, 0, 255);

        //DebugRendering::drawLine(renderer, Vectors::toVector2Int(screenCenter), Vectors::toVector2Int(center));
    }

    void update(float deltaTime)
    {

    }
};

SDL_Texture* Sphere::texture = nullptr;
bool Sphere::texturesLoaded = false;