//
// Created by timon on 12/2/25.
//

#include "VehicleLeave.h"

#include <SDL3/SDL_events.h>

#include "PlayerVehicleTracker.h"
#include "gameEvent/GameEvent.h"
#include "vehicles/Vehicle.h"

void Player::VehicleLeave::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
        if (!player_vehicle_tracker->player_is_in_vehicle)
            return;


        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_R) {
                if (player_vehicle_tracker->vehicle->canStopPiloting()) {
                    player_vehicle_tracker->vehicle->stopPiloting();
                }
            }
        }

}
