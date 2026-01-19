//
// Created by timon on 11/30/25.
//


#include "InteractableInteract.h"

#include "PlayerVehicleTracker.h"


void Player::InteractableInteract::enable() {
    enabled = true;
}

void Player::InteractableInteract::disable() {
    enabled = false;
    tooltip->disable();
}

void Player::InteractableInteract::onEnterOrLeaveVehicle(bool in_vehicle) {
    if (in_vehicle) {
        disable();
    }
    else {
        enable();
    }
}


void Player::InteractableInteract::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

IInteractable *Player::InteractableInteract::getIInteractableUnderMouse(const UpdateContext &context) {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    Vector2Int world_position = context.camera_info.screenToWorldPoint({mouse_x,mouse_y});

    auto hit = Physics::EntityPointCast(world_position,context.spaceShip);

    for (auto entity: hit) {
        if (auto * under_mouse = entity->asIInteractable()) {
            return under_mouse;
        }
    }
    return nullptr;
}

void Player::InteractableInteract::update(const UpdateContext &context) {
    if (player_vehicle_tracker->player_is_in_vehicle) {
        iinteractable_under_mouse = nullptr;
        return;
    }

    iinteractable_under_mouse = getIInteractableUnderMouse(context);

    if (iinteractable_under_mouse != nullptr) {
        tooltip->enable();
        tooltip->setPosition(context.camera_info.worldToScreenPoint(iinteractable_under_mouse->asEntity()->getPosition()));
        SDL_Color text_color = iinteractable_under_mouse->is_interactable(player_vehicle_tracker->player) ? SDL_Color(200,200,255,255) : SDL_Color(125,125,125,255);
        tooltip->setTextColor(text_color);
        tooltip->setText(iinteractable_under_mouse->getInteractionText());
    }
    else {
        tooltip->disable();
    }
}


void Player::InteractableInteract::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (player_vehicle_tracker->player_is_in_vehicle) {
        iinteractable_under_mouse = nullptr;
        return;
    }
    if (iinteractable_under_mouse == nullptr) {
        return;
    }


    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_E && iinteractable_under_mouse->is_interactable(player_vehicle_tracker->player)) {
            iinteractable_under_mouse->interact(player_vehicle_tracker->player);
        }
    }
}
