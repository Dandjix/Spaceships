#pragma once
#include "Behavior.h"
#include "json.hpp"
#include "../player/Camera.h"

class PlayerBehavior : public Behavior {
private:

public:
    explicit PlayerBehavior() = default;

    void update(const UpdateContext &context, PhysicsEntity *owner) override;


    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context, PhysicsEntity *owner) override;


    void spaceshipControlUpdate(const UpdateContext &context, PhysicsEntity *owner, SpaceShip *space_ship) override;

    void spaceshipControlHandleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context,
                                     PhysicsEntity *owner, SpaceShip *space_ship) override;

    constexpr bool isPlayerBehavior() override { return true; }

    nlohmann::json toJson();
};
