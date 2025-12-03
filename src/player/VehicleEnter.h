//
// Created by timon on 11/29/25.
//

#pragma once
#include <format>

#include "PlayerVehicleTracker.h"
#include "entities/Humanoid.h"
#include "../entities/scripts/ShortLivedEntity.h"
#include "userInterface/elements/entities/GUITooltip.h"
#include "vehicles/Vehicle.h"

namespace Player {
    class PlayerVehicleTracker;
}

namespace Player {
    class VehicleEnter : public ShortLivedEntity {
    private:
        GUITooltip * tooltip;

        PlayerVehicleTracker * player_vehicle_tracker;

        Events::Id on_player_in_vehicle_changed_id;
    protected:
        Vehicle * vehicle_under_mouse = nullptr;

        bool enabled;

    public:
        VehicleEnter(GUITooltip *tooltip,
            PlayerVehicleTracker *player_vehicle_tracker)
            : ShortLivedEntity({0,0}, 0),
              tooltip(tooltip),
              player_vehicle_tracker(player_vehicle_tracker),
              enabled(!player_vehicle_tracker->player_is_in_vehicle) {
            on_player_in_vehicle_changed_id = player_vehicle_tracker->on_player_in_vehicle_changed.subscribe([this](bool in_vehicle){onEnterOrLeaveVehicle(in_vehicle);});
        }

        ~VehicleEnter() override {
            // player_vehicle_tracker->on_player_in_vehicle_changed.unsubscribe(on_player_in_vehicle_changed_id); //We supposedly destroy both at the same time, no need for this nonsense
        }

    protected:
        void enable();

        void disable();


    public:

        void onEnterOrLeaveVehicle(bool in_vehicle);


        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        static Vehicle *getVehicleUnderMouse(const UpdateContext &context);

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;
    };

}
