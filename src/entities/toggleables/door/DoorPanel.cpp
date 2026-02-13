//
// Created by timon on 2/8/26.
//

#include "DoorPanel.h"

#include "entityRendering/RenderingInitialization.h"
#include "physics/shapes/RectPhysicsShape.h"
#include "physics/shapes/RectStaticPhysicsShape.h"
#include "rendering/util/RenderTexture.h"

SDL_Texture * DoorPanel::door_texture = nullptr;


DoorPanel::DoorPanel(const Vector2Int &position, const std::optional<float> &angle, Vector2Int dimensions)
    : ShortLivedPhysicsEntity(
        position,
        angle,
        new RectStaticPhysicsShape(this, dimensions)
    ),dimensions(dimensions) {
    std::cout << "door panel initialized " << getPosition().x << "," <<getPosition().y <<" , size : "<< dimensions.x << ", " << dimensions.y << std::endl;
}

void DoorPanel::registerInSpaceship(SpaceShip *space_ship) {
    ShortLivedPhysicsEntity::registerInSpaceship(space_ship);
}

DoorPanel *DoorPanel::initializeRendering(const EntityRendering::Context &context) {
    auto set = context.usage_map.subscribe("objects/door");
    door_texture = set->at("door");
    return this;
}

DoorPanel *DoorPanel::finalizeRendering(const EntityRendering::Context &context) {
    context.usage_map.unsubscribe("objects/door");
    return this;
}

void DoorPanel::render(SDL_Renderer *renderer, const RenderingContext &context) {
    Rendering::Util::renderTexture(renderer,context,getPosition(),getAngle(),door_texture,Vectors::toVector2Float(dimensions).scaleToScreenPosition());
}
