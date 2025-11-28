#pragma once
#include "../game/Update.h"
#include <SDL3/SDL.h>


namespace GameEvent {
    struct GameEventContext;
}

class PhysicsEntity;

class Behavior{

    public:
    virtual ~Behavior() = default;

    explicit Behavior()= default;

    virtual void update(const UpdateContext &context, PhysicsEntity * owner) = 0;
    virtual void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext & context, PhysicsEntity * owner) = 0;
};
