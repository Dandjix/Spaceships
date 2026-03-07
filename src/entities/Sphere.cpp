#include "Sphere.h"
#include <iostream>

#include "entityRendering/RenderingInitialization.h"
#include "physics/shapes/RoundPhysicsShape.h"
#include "rendering/util/RenderTexture.h"
#include "shipEditor/EntityPlacer/EntityPlacement/interface/Interface.h"
#include "textures/TextureSet.h"
#include "textures/UsageMap.h"


Sphere::Sphere(Vector2Int position, float radius) : PhysicsEntity(position, std::nullopt,
                                                                  new RoundPhysicsShape(this, radius)),
                                                    texture(nullptr), radius(radius) {
}

FROM_JSON_DEFINITION(Sphere) {
    return new Sphere(Vector2Int::fromJson(json["position"]), json["radius"]);
}

EDITOR_PLACE_DEFINITION(Sphere) {
    EntityPlacement::InterfaceForm::FormRequest form = {
        "Sphere",
        {
            {"A", EntityPlacement::InterfaceForm::INT},
            {"B", EntityPlacement::InterfaceForm::FLOAT},
            {"C", EntityPlacement::InterfaceForm::BOOL},
            {"D", EntityPlacement::InterfaceForm::VECTOR2INT},
            {"E", EntityPlacement::InterfaceForm::VECTOR2FLOAT},
            {"F", EntityPlacement::InterfaceForm::STRING},
        },
        {
            {"Z",{"A","B","C"}}
        }
    };

    Vector2Int pos = context->interface->getPosition();
    float angle = context->interface->getAngle();

    context->interface->askForForm(form,[pos, angle](const EntityPlacement::InterfaceForm::FormResult & result) {
        return new Sphere(pos,angle);
    });
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
    // Calculate the size of the sphere for rendering (scale it according to the camera)
    Vector2Float size(static_cast<float>(radius * 2), static_cast<float>(radius * 2));

    size = size.scaleToScreenPosition();

    // Render the texture with calculated size
    Rendering::Util::renderTexture(renderer, context, getPosition(), getAngle(), texture, size);
}

// Override update function (currently empty, but could be used to handle animations or interactions)
void Sphere::update(const UpdateContext &context) {
    // Add update logic if needed
}
