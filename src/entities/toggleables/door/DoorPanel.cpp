//
// Created by timon on 2/8/26.
//

#include "DoorPanel.h"

#include "entityRendering/RenderingInitialization.h"
#include "physics/shapes/RectPhysicsShape.h"
#include "physics/shapes/RectStaticPhysicsShape.h"

SDL_Texture * DoorPanel::door_texture = nullptr;


DoorPanel::DoorPanel(const Vector2Int &position, const std::optional<float> &angle, Vector2Int physics_dimensions)
    : ShortLivedPhysicsEntity(
        position,
        angle,
        new RectStaticPhysicsShape(this, physics_dimensions)
    ) {
    std::cout << "door panel initialized " << getPosition().x << "," <<getPosition().y <<" , size : "<< physics_dimensions.x << ", " << physics_dimensions.y << std::endl;
}

void DoorPanel::registerInSpaceship(SpaceShip *space_ship) {
    ShortLivedPhysicsEntity::registerInSpaceship(space_ship);
}

DoorPanel *DoorPanel::initializeRendering(const EntityRendering::Context &context) {
    auto set = context.usage_map.subscribe("objects/door");
    door_texture = set->at("floor");
    return this;
}

DoorPanel *DoorPanel::finalizeRendering(const EntityRendering::Context &context) {
    context.usage_map.unsubscribe("objects/door");
    return this;
}

void DoorPanel::render(SDL_Renderer *renderer, const RenderingContext &context) {
}
