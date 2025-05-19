#pragma once
#include <SDL3/SDL.h>
#include <optional>
#include "Vectors.h"
#include "Rendering.h"
#include "Update.h"


class Entity {
protected:
    Vector2Int position;  // Using Vector2 to represent position
    std::optional<float> angle; //if an angle is null, the object will always face towards the camera

    void renderTexture(SDL_Renderer* renderer, const RenderingContext& context, SDL_Texture* texture, Vector2Float destSize) const
    {
        Vector2Float center = context.toScreenPoint(position);

        SDL_FRect destRect = {
            center.x - destSize.x,
            center.y - destSize.y,
            destSize.x * 2.0f,
            destSize.y * 2.0f
        };
        float angle;
        if (hasAngle())
        {
            angle = getAngle() + context.cameraAngle;
        }
        else
        {
            angle = 0;
        }

        SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect, angle, nullptr, SDL_FLIP_NONE);
    }
public:
    /// <summary>
    /// creates an entity
    /// </summary>
    /// <param name="position">pixel position, converted internally to a position</param>
    Entity(Vector2Int position, std::optional<float> angle)
        : position(position.scaleToWorldPosition()), angle(angle){}  // Constructor using Vector2

    virtual ~Entity() {}

    virtual void update(const UpdateContext & context) = 0;
    virtual void handleEvent(const SDL_Event & event){}
    virtual void render(SDL_Renderer* renderer, const RenderingContext& context) = 0;
    virtual void debugRender(SDL_Renderer* renderer, const RenderingContext& context){}

    Vector2Int getPosition() const {
        return position;
    }



    // accessors
    int getX() const { return position.x; }
    int getY() const { return position.y; }
    bool hasAngle() const { return angle.has_value(); }
    float getAngle() const { return angle.value_or(0); }
    void setPosition(Vector2Int newPosition) { position = newPosition; }
    void setAngle(float a) {
        if (!angle.has_value())
        {
            throw std::invalid_argument("this entity was instantiated without an angle, it is not possible to assign it one now.");
        }
        float new_angle = static_cast<float>(fmod(a + 360, 360));
        angle = new_angle;
    }
};
