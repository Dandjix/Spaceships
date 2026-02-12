//
// Created by timon on 7/9/25.
//

#include "Entity.h"

#include "game/Rendering.h"
#include "physics/scripts/PhysicsEntity.h"
#include "spaceships/SpaceShip.h"

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

void Entity::unregisterInSpaceship(SpaceShip* space_ship, bool delete_when_done)
{
    space_ship->entities.add_to_erasing_queue(this,delete_when_done);
    onUnRegistered(space_ship);
}

void Entity::movePosition(Vector2Float delta) {
    if (delta.sqrLength() <= 0)
        return;

    // auto old = getPosition();

    this->setPosition(this->getPosition() + Vectors::toVector2Int(delta));

    // std::cout << "MOVEMENT DID OCCUR :" << getPosition()-old <<  std::endl;
}

void Entity::kill(SpaceShip *space_ship) {
    on_killed.emit(space_ship);
    unregisterInSpaceship(space_ship,true);
}

