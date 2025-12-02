//
// Created by timon on 11/29/25.
//

#pragma once
#include <format>

#include "entities/Humanoid.h"
#include "entities/ShortLivedEntity.h"
#include "userInterface/elements/entities/GUITooltip.h"
#include "vehicles/Vehicle.h"

namespace Player {
    class VehicleEnter : public ShortLivedEntity {
    private:
        GUITooltip * tooltip;

    protected:
        Humanoid *player = nullptr;

        Vehicle *vehicle = nullptr;

        Vehicle * vehicle_under_mouse = nullptr;

        Events::Id start_piloting_event_id = Events::null_id;
        Events::Id stop_piloting_event_id = Events::null_id;

        bool enabled;

        void determinePlayerAndVehicle(Entity *new_player);

        void subscribeEvents();

        void unsubscribeEvents() const;

        void player_status_changed(Entity *player_or_vehicle);

        void enable();

        void disable();

        void setStatus();

    public:

        explicit VehicleEnter(Entity *player_or_vehicle, GUITooltip * tooltip, bool enabled = true)
            : ShortLivedEntity({0, 0}, 0), tooltip(tooltip) {

            determinePlayerAndVehicle(player_or_vehicle);
            subscribeEvents();
            setStatus();
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        static Vehicle *getVehicleUnderMouse(const UpdateContext &context);

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;
    };

}
