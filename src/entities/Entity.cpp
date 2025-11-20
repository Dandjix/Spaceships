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

    auto old = getPosition();

    this->setPosition(this->getPosition()+ Vectors::toVector2Int(delta));

    std::cout << "MOVEMENT DID OCCUR :" << getPosition()-old <<  std::endl;
}
