#pragma once
#include "Behavior.h"
#include "../player/Camera.h"

class PlayerBehavior : public Behavior{
private:

    public:
    explicit PlayerBehavior() {
    }
    void update(const UpdateContext &context, PhysicsEntity * owner) override;
    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext & context, PhysicsEntity * owner) override;

    nlohmann::json toJson() override;
};

