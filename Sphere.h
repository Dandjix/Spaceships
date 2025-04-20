#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Vectors.h"
#include "Rendering.h"

class Sphere : public RoundEntity {
private:
    static SDL_Texture* texture;  // Texture for the sphere
    static bool texturesLoaded;   // Flag to check if textures are loaded

public:
    // Constructor
    Sphere(Vector2Int position, int radius);

    // Static function to load textures
    static void LoadTextures(SDL_Renderer* renderer);

    // Override render function to render the sphere
    void render(SDL_Renderer* renderer, const RenderingContext& context) override;

    // Override update function (currently does nothing)
    void update(float deltaTime) override;
};
