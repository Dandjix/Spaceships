#pragma once
#include "../game/Update.h"
#include <SDL3/SDL.h>

#include "events/Event.h"


namespace GameEvent {
    struct GameEventContext;
}

class PhysicsEntity;

class Behavior {
public:
    enum Control {
        OnFoot,
        PilotingVehicle,
        PilotingSpaceship
    };

    /**
     * Called when a behavior changes to a control type
     * @param control the control enum, which can take values defined in Behavior.h above this method
     */
    virtual void ChangeControl(Control control){}

    virtual ~Behavior() = default;

    virtual nlohmann::json toJson() =0;

    static Behavior *fromJson(const nlohmann::basic_json<> &value);

    explicit Behavior() = default;

    virtual bool constexpr isPlayerBehavior() { return false; }

    /**
     * This is called when the entity is either walking on foot or piloting a vehicle
     * @param context
     * @param owner
     */
    virtual void update(const UpdateContext &context, PhysicsEntity *owner) = 0;

    virtual void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context,
                             PhysicsEntity *owner) = 0;

    /**
     * This is called when piloting a spaceship
     * @param context the update context, containing delta time, camera info and the like
     * @param owner the owner entity
     * @param space_ship the space ship being piloted
     */
    virtual void spaceshipControlUpdate(const UpdateContext &context, PhysicsEntity *owner, SpaceShip *space_ship) = 0;

    virtual void spaceshipControlHandleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context,
                                             PhysicsEntity *owner, SpaceShip *space_ship) = 0;
};
