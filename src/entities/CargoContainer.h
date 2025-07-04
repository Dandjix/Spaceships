#pragma once
#include "BoxEntity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../math/Vectors.h"
#include "../game/Rendering.h"

class CargoContainer : public BoxEntity {
public:
    enum class Variation {
        blank,
        EMA,
        SL,
        SN,
        COUNT // count is at the end and allows to know the number of enum values
    };

private:
    static SDL_Texture* textures[static_cast<int>(Variation::COUNT)];
    static bool texturesLoaded;

    Variation variation;
    SDL_Color color;

public:
    // Constructor
    CargoContainer(Vector2Int position, float angle = 0.0f, Variation variation = Variation::blank);

    // Static method to load textures
    static void LoadTextures(SDL_Renderer* renderer);

    // Override update method (may not be necessary for CargoContainer)
    void update(const UpdateContext & context) override;

    // Override render method
    void render(SDL_Renderer* renderer, const RenderingContext& context) override;

private:
    // Helper method to generate random color
    SDL_Color getRandomColor();
};

