//
// Created by timon on 11/30/25.
//


#include "VehicleEnter.h"

inline void Player::VehicleEnter::determinePlayerAndVehicle(Entity *new_player) {
    player = nullptr;
    vehicle = nullptr;

    if (auto humanoid = dynamic_cast<Humanoid *>(new_player)) {
        player = humanoid;
        setStatus();
        return;
    }

    if (auto player_vehicle = dynamic_cast<Vehicle *>(new_player)) {
        player = player_vehicle->getPilot();
        vehicle = player_vehicle;
        setStatus();
        return;
    }

    throw std::invalid_argument(
        "variable pointer " + std::to_string(reinterpret_cast<uintptr_t>(new_player)) +
        " is neither a humanoid nor a vehicle"
    );
}

void Player::VehicleEnter::subscribeEvents() {
    start_piloting_event_id = player->on_start_piloting_vehicle.subscribe([this](Vehicle *started) {
        player_status_changed(started);
    });
    stop_piloting_event_id = player->on_stop_piloting_vehicle.subscribe([this](Vehicle *_) {
        player_status_changed(this->player);
    });
}

void Player::VehicleEnter::unsubscribeEvents() const {
    player->on_start_piloting_vehicle.unsubscribe(start_piloting_event_id);
    player->on_stop_piloting_vehicle.unsubscribe(stop_piloting_event_id);
}

void Player::VehicleEnter::player_status_changed(Entity *player_or_vehicle) {
    unsubscribeEvents();

    determinePlayerAndVehicle(player_or_vehicle);

    subscribeEvents();
}

void Player::VehicleEnter::enable() {
    enabled = true;
}

void Player::VehicleEnter::disable() {
    enabled = false;
    tooltip->disable();
}

void Player::VehicleEnter::setStatus() {
    if (vehicle == nullptr) {
        enable();
    }
    else {
        disable();
    }
}

void Player::VehicleEnter::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

Vehicle *Player::VehicleEnter::getVehicleUnderMouse(const UpdateContext &context) {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    Vector2Int world_position = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});

    auto hit = Physics::EntityPointCast(world_position,context.spaceShip);

    for (auto entity: hit) {
        if (auto * vehicle_under_mouse = dynamic_cast<Vehicle *>(entity)) {
            return vehicle_under_mouse;
        }
    }
    return nullptr;
}

void Player::VehicleEnter::update(const UpdateContext &context) {
    if (vehicle!=nullptr || player==nullptr) {
        vehicle_under_mouse = nullptr;
        return;
    }

    vehicle_under_mouse = getVehicleUnderMouse(context);

    if (vehicle_under_mouse != nullptr) {
        tooltip->enable();
        tooltip->setPosition(context.camera_info.worldToScreenPoint(vehicle_under_mouse->getPosition()));
        SDL_Color text_color = vehicle_under_mouse->canStartPiloting(player) ? SDL_Color(200,200,255,255) : SDL_Color(125,125,125,255);
        tooltip->setTextColor(text_color);
        tooltip->setText("[E] Enter "+vehicle_under_mouse->getVehicleName());
    }
    else {
        tooltip->disable();
    }
}


void Player::VehicleEnter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (vehicle!=nullptr || player==nullptr) {
        vehicle_under_mouse = nullptr;
        return;
    }
    if (vehicle_under_mouse == nullptr) {
        return;
    }


    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_E && vehicle_under_mouse->canStartPiloting(player)) {
            vehicle_under_mouse->startPiloting(player);
        }
    }
}
