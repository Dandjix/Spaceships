//
// Created by timon on 11/29/25.
//

#include <format>

#include "Vehicle.h"

void Vehicle::assumeControl(Humanoid * new_pilot) {
    if (new_pilot != nullptr && new_pilot->getBehavior() != nullptr)
        new_pilot->getBehavior()->ChangeControl(getControlType());
}

void Vehicle::relinquishControl(Humanoid * old_pilot) {
    if (old_pilot != nullptr && old_pilot->getBehavior() != nullptr)
        old_pilot->getBehavior()->ChangeControl(Behavior::OnFoot);
}

void Vehicle::onRegistered(SpaceShip *newSpaceship) {
    spaceship = newSpaceship;
}

void Vehicle::interact(Humanoid *activator, const GameEvent::GameEventContext &context) {
    stopPiloting();

    pilot = activator;
    spaceship->unregisterEntities({pilot});

    if (is_player()) {
        spaceship->setPlayer(this);
    }

    assumeControl(pilot);
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

    relinquishControl(old_pilot);
    on_ownership_change.emit(old_pilot);
}

bool Vehicle::is_interactable(Humanoid *activator) {
    if (pilot != nullptr)
        return false;

    return IInteractable::distance_check(this,activator);
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

std::string Vehicle::getInteractionText() const {
    return std::format("[E] enter {}",getVehicleName());
}

constexpr bool Vehicle::is_player() {
    if (pilot == nullptr)
        return false;

    return pilot->is_player();
}
