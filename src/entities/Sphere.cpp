#include "Sphere.h"
#include <iostream>

#include "physics/shapes/RoundPhysicsShape.h"
#include "textures/TextureSet.h"
#include "textures/UsageMap.h"


Sphere::Sphere(Vector2Int position, float radius): PhysicsEntity(position,  std::nullopt,new RoundPhysicsShape(this,radius)), radius(radius) {
    texture_set = Textures::UsageMap::getInstance().subscribe("objects/sphere");
}

Sphere::~Sphere() {
    Textures::UsageMap::getInstance().unsubscribe("objects/sphere");
}

// Override render function
void Sphere::render(SDL_Renderer* renderer, const RenderingContext& context) {
    if (!texture_set) {
        std::cerr << "Texture not loaded!" << std::endl;
        return;
    }

    // Calculate the half-size of the sphere for rendering (scale it according to the camera)
    Vector2Float halfSize(static_cast<float>(radius), static_cast<float>(radius));

    halfSize = halfSize.scaleToScreenPosition();

    // Render the texture with calculated size
    renderTexture(renderer, context, texture_set->at("1"), halfSize);
}

// Override update function (currently empty, but could be used to handle animations or interactions)
void Sphere::update(const UpdateContext & context) {
    // Add update logic if needed
}
