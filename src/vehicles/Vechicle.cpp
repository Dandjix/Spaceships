//
// Created by timon on 11/29/25.
//

#include "Vehicle.h"

void Vehicle::onRegistered(SpaceShip *newSpaceship) {
    spaceship = newSpaceship;
}

void Vehicle::startPiloting(Humanoid *newPilot) {
    stopPiloting();

    spaceship->unregisterEntities({pilot});
    pilot = newPilot;

    if (is_player()) {
        spaceship->setPlayer(this);
    }
}

void Vehicle::stopPiloting() {
    if(pilot == nullptr) return;


    spaceship->registerEntities({pilot});
    pilot->setPosition(getPosition());
    pilot->setAngle(getAngle());

    if (pilot->is_player()) {
        spaceship->setPlayer(pilot);
    }

    pilot = nullptr;
}

nlohmann::json Vehicle::toJson() {
    auto json = PhysicsEntity::toJson();
    if (pilot != nullptr) {
        json["pilot"] = pilot->toJson();
    }
    return json;
}

void Vehicle::update(const UpdateContext &context) {
    PhysicsEntity::update(context);
    if (pilot == nullptr || pilot->getBehavior() == nullptr) {
        return;
    }
    pilot->getBehavior()->update(context,this);
}

void Vehicle::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    PhysicsEntity::handleEvent(event, context);
    if (pilot == nullptr || pilot->getBehavior() == nullptr) {
        return;
    }
    pilot->getBehavior()->handleEvent(event,context,this);
}

constexpr bool Vehicle::is_player() {
    if (pilot == nullptr)
        return false;

    return pilot->is_player();
}
