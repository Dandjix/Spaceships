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
    if (hasAngle())entry["angle"] = getAngle();
    entry["type"] = getJsonType();
    return entry;
}

void Entity::update(const UpdateContext &context) {}

void Entity::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {}

void Entity::registerInInstance(Instances::Instance *world_instance)
{
    world_instance->entities.insert(this);
    onRegistered(world_instance);
}

void Entity::unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done)
{
    world_instance->entities.add_to_erasing_queue(this,delete_when_done);
    onUnRegistered(world_instance);
}

void Entity::movePosition(Vector2Float delta) {
    if (delta.sqrLength() <= 0)
        return;

    // auto old = getPosition();

    this->setPosition(this->getPosition() + Vectors::toVector2Int(delta));

    // std::cout << "MOVEMENT DID OCCUR :" << getPosition()-old <<  std::endl;
}

void Entity::kill(Instances::Instance *world_instance) {
    on_killed.emit(world_instance);
    unregisterInInstance(world_instance,true);
}

