#pragma once
#include "../game/Update.h"
#include <SDL3/SDL.h>

class Entity;

class Behavior{

    public:
    virtual ~Behavior() = default;

    explicit Behavior()= default;

    virtual void update(const UpdateContext &context, Entity * owner) = 0;
    virtual void handleEvent(const SDL_Event &event, Entity * owner) = 0;
};
