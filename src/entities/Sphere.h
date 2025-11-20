#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "BoxEntity.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"

class Sphere : public BoxEntity {
private:
    static SDL_Texture* texture;  // Texture for the sphere
    static bool texturesLoaded;   // Flag to check if textures are loaded
    float radius;
public:
    // Constructor
    Sphere(Vector2Int position, float radius, Behavior * behavior)
        : BoxEntity(position, {radius, radius}, std::nullopt, behavior), radius(radius)
    {
    }

    // Static function to load textures
    static void LoadTextures(SDL_Renderer* renderer);

    // Override render function to render the sphere
    void render(SDL_Renderer* renderer, const RenderingContext& context) override;

    // Override update function (currently does nothing)
    void update(const UpdateContext & context) override;
};
