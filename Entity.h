#pragma once
#include <SDL3/SDL.h>
#include "Vector2Int.h"  // Assuming the Vector2 header is correctly included

class Entity {
protected:
    Vector2Int position;  // Using Vector2 to represent position

public:
    Entity(int x = 0.0f, int y = 0.0f)
        : position(x, y) {}  // Constructor using Vector2 constructor

    Entity(Vector2Int position)
        : position(position) {}  // Constructor using Vector2

    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Position accessors
    int getX() const { return position.x; }
    int getY() const { return position.y; }
    void setPosition(int newX, int newY) { position = Vector2Int(newX, newY); }
    void setPosition(Vector2Int newPosition) { position = newPosition; }
};
