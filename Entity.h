#pragma once
#include <SDL3/SDL.h>

class Entity {
protected:
    float x, y; // <-- Add position here

public:
    Entity(float x = 0.0f, float y = 0.0f)
        : x(x), y(y) {}

    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Position accessors
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }
};
