//
// Created by timon on 12/2/25.
//

#pragma once
#include "entities/Humanoid.h"
#include "../entities/scripts/ShortLivedEntity.h"
#include "vehicles/Vehicle.h"

namespace Player {
    class PlayerVehicleTracker : public ShortLivedEntity {
        Events::Id start_piloting_event_id = Events::null_id;
        Events::Id stop_piloting_event_id = Events::null_id;

    public:
        Humanoid *player = nullptr;

        Vehicle *vehicle = nullptr;

        bool player_is_in_vehicle;

        Event<bool> on_player_in_vehicle_changed;

        void player_status_changed(Entity *player_or_vehicle);

        void determinePlayerAndVehicle(Entity *new_player);

        explicit PlayerVehicleTracker(Entity *player_or_vehicle)
            : ShortLivedEntity({0, 0}, 0) {
            determinePlayerAndVehicle(player_or_vehicle);
            player_is_in_vehicle = vehicle != nullptr;
            subscribeEvents();
        }

        void setPlayerIsInVehicle();

        void subscribeEvents();

        void unsubscribeEvents() const;

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {}
    };
}
