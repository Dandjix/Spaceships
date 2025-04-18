#pragma once
#include <SDL3/SDL.h>
#include <optional>
#include "Vectors.h"
#include "Rendering.h"


class Entity {
protected:
    Vector2Int position;  // Using Vector2 to represent position
    std::optional<float> angle; //if an angle is null, the object will always face towards the camera
public:
    /// <summary>
    /// creates an entity
    /// </summary>
    /// <param name="position">pixel position, converted internally to a position</param>
    Entity(Vector2Int position, std::optional<float> angle)
        : position(Vectors::toWorldPosition(position)), angle(angle){}  // Constructor using Vector2

    virtual ~Entity() {}

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer, const RenderingContext context) = 0;
    virtual void debugRender(SDL_Renderer* renderer, const RenderingContext context){}

    Vector2Int getPosition() {
        return position;
    }



    // accessors
    int getX() const { return position.x; }
    int getY() const { return position.y; }
    float getAngle() const { return angle.value_or(0); }
    void setPosition(Vector2Int newPosition) { position = newPosition; }
    void setAngle(float a) {
        if (!angle.has_value())
        {
            throw std::invalid_argument("this entity was instantiated without an angle, it is not possible to assign it one now.");
        }
        angle = a; 
    }
};
