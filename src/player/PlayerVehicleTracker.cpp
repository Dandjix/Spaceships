//
// Created by timon on 12/2/25.
//

#include "PlayerVehicleTracker.h"

#include "entities/Humanoid.h"


inline void Player::PlayerVehicleTracker::determinePlayerAndVehicle(Entity *new_player) {
    player = nullptr;
    vehicle = nullptr;

    if (auto humanoid = dynamic_cast<Humanoid *>(new_player)) {
        player = humanoid;
        return;
    }

    if (auto player_vehicle = dynamic_cast<Vehicle *>(new_player)) {
        player = player_vehicle->getPilot();
        vehicle = player_vehicle;
        return;
    }

    throw std::invalid_argument(
        "variable pointer " + std::to_string(reinterpret_cast<uintptr_t>(new_player)) +
        " is neither a humanoid nor a vehicle"
    );
}

void Player::PlayerVehicleTracker::subscribeEvents() {
    start_piloting_event_id = player->on_start_piloting_vehicle.subscribe([this](Vehicle *started) {
        player_status_changed(started);
    });
    stop_piloting_event_id = player->on_stop_piloting_vehicle.subscribe([this](Vehicle *_) {
        player_status_changed(this->player);
    });
}

void Player::PlayerVehicleTracker::unsubscribeEvents() const {
    player->on_start_piloting_vehicle.unsubscribe(start_piloting_event_id);
    player->on_stop_piloting_vehicle.unsubscribe(stop_piloting_event_id);
}

void Player::PlayerVehicleTracker::player_status_changed(Entity *player_or_vehicle) {
    unsubscribeEvents();

    determinePlayerAndVehicle(player_or_vehicle);
    setPlayerIsInVehicle();

    subscribeEvents();
}

void Player::PlayerVehicleTracker::setPlayerIsInVehicle() {
    bool in_vehicle = vehicle != nullptr;

    if (in_vehicle == player_is_in_vehicle)
        return;

    player_is_in_vehicle = in_vehicle;
    on_player_in_vehicle_changed.emit(in_vehicle);
}