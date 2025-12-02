//
// Created by timon on 11/30/25.
//


#include "VehicleEnter.h"

#include "PlayerVehicleTracker.h"


void Player::VehicleEnter::enable() {
    enabled = true;
}

void Player::VehicleEnter::disable() {
    enabled = false;
    tooltip->disable();
}

void Player::VehicleEnter::onEnterOrLeaveVehicle(bool in_vehicle) {
    if (in_vehicle) {
        disable();
    }
    else {
        enable();
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
    if (player_vehicle_tracker->player_is_in_vehicle) {
        vehicle_under_mouse = nullptr;
        return;
    }

    vehicle_under_mouse = getVehicleUnderMouse(context);

    if (vehicle_under_mouse != nullptr) {
        tooltip->enable();
        tooltip->setPosition(context.camera_info.worldToScreenPoint(vehicle_under_mouse->getPosition()));
        SDL_Color text_color = vehicle_under_mouse->canStartPiloting(player_vehicle_tracker->player) ? SDL_Color(200,200,255,255) : SDL_Color(125,125,125,255);
        tooltip->setTextColor(text_color);
        tooltip->setText("[E] Enter "+vehicle_under_mouse->getVehicleName());
    }
    else {
        tooltip->disable();
    }
}


void Player::VehicleEnter::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (player_vehicle_tracker->player_is_in_vehicle) {
        vehicle_under_mouse = nullptr;
        return;
    }
    if (vehicle_under_mouse == nullptr) {
        return;
    }


    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_E && vehicle_under_mouse->canStartPiloting(player_vehicle_tracker->player)) {
            vehicle_under_mouse->startPiloting(player_vehicle_tracker->player);
        }
    }
}
