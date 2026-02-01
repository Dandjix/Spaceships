#include "Sphere.h"
#include <iostream>

#include "entityRendering/RenderingInitialization.h"
#include "physics/shapes/RoundPhysicsShape.h"
#include "textures/TextureSet.h"
#include "textures/UsageMap.h"


Sphere::Sphere(Vector2Int position, float radius) : PhysicsEntity(position, std::nullopt,
                                                                  new RoundPhysicsShape(this, radius)),
                                                    texture(nullptr), radius(radius) {
}

FROM_JSON_DEFINITION(Sphere) {
    return new Sphere(Vector2Int::fromJson(json["position"]), json["radius"]);
}

Sphere *Sphere::initializeRendering(const EntityRendering::Context &context) {
    texture = context.usage_map.subscribe("objects/sphere")->at("1");
    return this;
}

Entity *Sphere::finalizeRendering(const EntityRendering::Context &context) {
    context.usage_map.unsubscribe("objects/sphere");
    return this;
}

// Override render function
void Sphere::render(SDL_Renderer *renderer, const RenderingContext &context) {
    // Calculate the half-size of the sphere for rendering (scale it according to the camera)
    Vector2Float halfSize(static_cast<float>(radius), static_cast<float>(radius));

    halfSize = halfSize.scaleToScreenPosition();

    // Render the texture with calculated size
    renderTexture(renderer, context, texture, halfSize);
}

// Override update function (currently empty, but could be used to handle animations or interactions)
void Sphere::update(const UpdateContext &context) {
    // Add update logic if needed
}
