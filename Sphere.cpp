#include "Sphere.h"
#include <iostream>

// Initialize static members
SDL_Texture* Sphere::texture = nullptr;
bool Sphere::texturesLoaded = false;

Sphere::Sphere(Vector2Int position, int radius)
    : RoundEntity(position, std::nullopt, radius) {
}

// Static function to load textures
void Sphere::LoadTextures(SDL_Renderer* renderer) {
    if (texturesLoaded) return;

    texture = IMG_LoadTexture(renderer, "assets/textures/objects/spheres/1.png");

    if (!texture) {
        std::cerr << "Failed to load texture: " << SDL_GetError() << std::endl;
    }

    texturesLoaded = true;
}

// Override render function
void Sphere::render(SDL_Renderer* renderer, const RenderingContext& context) {
    if (!texture) {
        std::cerr << "Texture not loaded!" << std::endl;
        return;
    }

    // Calculate the half-size of the sphere for rendering (scale it according to the camera)
    Vector2Float halfSize(static_cast<float>(radius), static_cast<float>(radius));
    halfSize = halfSize / context.cameraScale;

    // Render the texture with calculated size
    renderTexture(renderer, context, texture, halfSize);
}

// Override update function (currently empty, but could be used to handle animations or interactions)
void Sphere::update(float deltaTime) {
    // Add update logic if needed
}
