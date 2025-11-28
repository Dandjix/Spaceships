#pragma once
#include "Behavior.h"
#include "../player/Camera.h"

class PlayerBehavior : public Behavior{
private:
    Camera * camera;

    public:
    explicit PlayerBehavior(Camera * camera): camera(camera) {
    }
    void update(const UpdateContext &context, PhysicsEntity * owner) override;
    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext & context, PhysicsEntity * owner) override;
};

