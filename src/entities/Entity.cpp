//
// Created by timon on 7/9/25.
//

#include "Entity.h"

void Entity::renderTexture(SDL_Renderer *renderer, const RenderingContext &context, SDL_Texture *texture,
    Vector2Float destSize) const {
    destSize = destSize / context.camera_info.cameraScale;

    Vector2Float center = context.camera_info.worldToScreenPoint(position);

    SDL_FRect destRect = {
        center.x - destSize.x,
        center.y - destSize.y,
        destSize.x * 2.0f,
        destSize.y * 2.0f};
    float entity_angle;
    if (hasAngle())
    {
        entity_angle = getAngle() + context.camera_info.cameraAngle;
    }
    else
    {
        entity_angle = 0;
    }

    SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, entity_angle, nullptr, SDL_FLIP_NONE);
}

nlohmann::json Entity::toJson() {
    nlohmann::json entry = {};
    entry["position"] = getPosition().toJson();
    if (hasAngle()) {
        entry["angle"] = getAngle();
    }
    entry["type"] = getJsonType();
    return entry;
}

void Entity::update(const UpdateContext &context) {}

void Entity::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {}

void Entity::registerInSpaceship(SpaceShip* space_ship)
{
    space_ship->entities.insert(this);
    onRegistered(space_ship);
}

void Entity::unregisterInSpacehip(SpaceShip* space_ship)
{
    space_ship->entities.erase(this);
    onUnRegistered(space_ship);
}

void Entity::movePosition(Vector2Float delta, SpaceShip * space_ship) {
    if (delta.sqrLength() <= 0)
        return;

    // auto old = getPosition();

    this->setPosition(this->getPosition() + Vectors::toVector2Int(delta));

    // std::cout << "MOVEMENT DID OCCUR :" << getPosition()-old <<  std::endl;
}

