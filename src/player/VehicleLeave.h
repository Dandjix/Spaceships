//
// Created by timon on 12/2/25.
//

#pragma once
#include "PlayerVehicleTracker.h"
#include "../entities/scripts/ShortLivedEntity.h"

namespace Player {
    class VehicleLeave : public ShortLivedEntity {
    private:
        PlayerVehicleTracker *player_vehicle_tracker;

    public:
        explicit VehicleLeave(PlayerVehicleTracker *player_vehicle_tracker)
            : ShortLivedEntity({0, 0}, 0), player_vehicle_tracker(player_vehicle_tracker) {
        }

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override {
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

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {}

        VehicleLeave * initializeRendering(const EntityRendering::Context &context) override {return this;}

        VehicleLeave * finalizeRendering(const EntityRendering::Context &context) override {return this;}
    };
}
