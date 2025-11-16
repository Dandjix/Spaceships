//
// Created by timon on 7/9/25.
//

#include "Entity.h"

void Entity::update(const UpdateContext &context) {
    if (behavior == nullptr)
        return;
    behavior->update(context,this);
}

void Entity::handleEvent(const SDL_Event &event) {
    if (behavior == nullptr)
        return;
    behavior->handleEvent(event,this);
}

void Entity::movePosition(Vector2Float delta, SpaceShip * space_ship) {
    this->setPosition(this->getPosition()+ Vectors::toVector2Int(delta.scaleToWorldPosition()));
}
