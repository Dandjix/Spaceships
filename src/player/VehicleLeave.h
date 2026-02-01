//
// Created by timon on 12/2/25.
//

#pragma once
#include "entities/scripts/ShortLivedEntity.h"

namespace Player {
    class PlayerVehicleTracker;

    class VehicleLeave : public ShortLivedEntity {
    private:
        PlayerVehicleTracker *player_vehicle_tracker;

    public:
        explicit VehicleLeave(PlayerVehicleTracker *player_vehicle_tracker)
            : ShortLivedEntity({0, 0}, 0), player_vehicle_tracker(player_vehicle_tracker) {
        }

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override ;
        void render(SDL_Renderer *renderer, const RenderingContext &context) override {}

        VehicleLeave * initializeRendering(const EntityRendering::Context &context) override {return this;}

        VehicleLeave * finalizeRendering(const EntityRendering::Context &context) override {return this;}
    };
}
