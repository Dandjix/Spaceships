#pragma once
#include <SDL3/SDL.h>
#include "Vector2.cpp"  // Assuming the Vector2 header is correctly included

class Entity {
protected:
    Vector2 position;  // Using Vector2 to represent position

public:
    Entity(float x = 0.0f, float y = 0.0f)
        : position(x, y) {}  // Constructor using Vector2 constructor

    Entity(Vector2 position)
        : position(position) {}  // Constructor using Vector2

    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Position accessors
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    void setPosition(float newX, float newY) { position = Vector2(newX, newY); }
    void setPosition(Vector2 newPosition) { position = newPosition; }
};
