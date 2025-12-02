//
// Created by timon on 11/29/25.
//

#include "Vehicle.h"

void Vehicle::onRegistered(SpaceShip *newSpaceship) {
    spaceship = newSpaceship;
}

void Vehicle::startPiloting(Humanoid *newPilot) {
    stopPiloting();
    
    pilot = newPilot;
    spaceship->unregisterEntities({pilot});

    if (is_player()) {
        spaceship->setPlayer(this);
    }

    pilot->on_start_piloting_vehicle.emit(this);
    pilot->on_ownership_change.emit(this);
}

void Vehicle::stopPiloting() {
    if(pilot == nullptr) return;


    spaceship->registerEntities({pilot});
    pilot->setPosition(getPosition());
    pilot->setAngle(getAngle());

    if (pilot->is_player()) {
        spaceship->setPlayer(pilot);
    }
    pilot->on_stop_piloting_vehicle.emit(this);
    auto old_pilot = pilot;
    pilot = nullptr;

    on_ownership_change.emit(old_pilot);
}

bool Vehicle::canStartPiloting(Humanoid *newPilot) {
    float distance = (newPilot->getPosition() - position).length();
    float max_distance = Scaling::scaleToWorld(128.0f);

    if (distance > max_distance || pilot != nullptr){
        return false;
    }

    auto raycast_hit = Physics::RayCast(newPilot->getPosition(),Vectors::toVector2Float(position - newPilot->getPosition()),spaceship,distance);
    if (raycast_hit.hit) {
        return false;
    }
    return true;
}

bool Vehicle::canStopPiloting() {
    return true;
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

void Vehicle::setBehavior(Behavior *value) {
    if (pilot == nullptr) {
        throw std::runtime_error("Tried to set behavior on vehicle without a pilot : this won't work");
    }
    pilot->setBehavior(value);
}

Behavior * Vehicle::getBehavior() const {
    if (pilot != nullptr) {
        return pilot->getBehavior();
    }
    return nullptr;
}

constexpr bool Vehicle::is_player() {
    if (pilot == nullptr)
        return false;

    return pilot->is_player();
}
