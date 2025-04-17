#pragma once
#include "BoxEntity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Vectors.h"
#include "Vector2Float.h"
#include "Vector2Int.h"

class CargoContainer : public BoxEntity {
public :
    enum class Variation {
        blank,
        EMA,
        SL,
        SN,
        COUNT //count is at the end and allows to know the number of enum values
    };
private:
    static SDL_Texture* textures[static_cast<int>(Variation::COUNT)];
    static bool texturesLoaded;

    Variation variation;
    SDL_Color color;
public:
    static void LoadTextures(SDL_Renderer* renderer) {
        if (texturesLoaded) return;

        textures[0] = IMG_LoadTexture(renderer, "assets/textures/objects/cargoContainers/blank.png");
        textures[1] = IMG_LoadTexture(renderer, "assets/textures/objects/cargoContainers/EMA.png");
        textures[2] = IMG_LoadTexture(renderer, "assets/textures/objects/cargoContainers/SL.png");
        textures[3] = IMG_LoadTexture(renderer, "assets/textures/objects/cargoContainers/SN.png");

        for (int i = 0; i < static_cast<int>(Variation::COUNT); i++)
        {
            if (textures[i] == NULL)
            {
                std::cout << "texture " << i << "is null.\n";
            }
        }

        texturesLoaded = true;
    }

    CargoContainer(int x, int y, float angle = 0.0f, Variation variation = Variation::blank)
        : BoxEntity(x, y, 61, 24, angle),variation(variation) {
        color = getRandomColor();
    }

    void update(float deltaTime) override {
        // CargoContainer might not need to move, so just leave it empty.
    }

    void render(SDL_Renderer* renderer, const Vector2Float cameraPos, int screenWidth, int screenHeight) override {
        SDL_Texture* texture = textures[static_cast<int>(variation)];
        if (!texture) {
            //std::cout << "could not find textures : " << textures;
            return;
        }

        // Apply per-instance tint
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

        Vector2Float center = toVector2Float(position) - cameraPos;
        Vector2Float halfSize = toVector2Float(scale, true) * 0.5f;

        SDL_FRect destRect = {
            center.x - halfSize.x,
            center.y - halfSize.y,
            halfSize.x * 2.0f,
            halfSize.y * 2.0f
        };

        SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, angle, nullptr, SDL_FLIP_NONE);
    }
private :
    SDL_Color getRandomColor()
    {
        unsigned char r = 128 + rand() % 127;
        unsigned char g = 128 + rand() % 127;
        unsigned char b = 128 + rand() % 127;

        return { r,g,b,255 };
    }

};

SDL_Texture* CargoContainer::textures[static_cast<int>(CargoContainer::Variation::COUNT)] = {};
bool CargoContainer::texturesLoaded = false;