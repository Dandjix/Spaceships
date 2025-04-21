#pragma once
#include "Camera.h"

class FreeCamera : public Camera
{
protected:
    float speed;
public:
    FreeCamera(Vector2Int position,float angle,float scale, float speed) : Camera(position,angle,scale),speed(speed) {}

    void update(float deltaTime) override {
        const bool* state = SDL_GetKeyboardState(NULL);
        float deltaX = 0;
        float deltaY = 0;
        if (state[SDL_SCANCODE_W]) {
            deltaY -= 1;
        }
        if (state[SDL_SCANCODE_S]) {
            deltaY += 1;
        }
        if (state[SDL_SCANCODE_A]) {
            deltaX -= 1;
        }
        if (state[SDL_SCANCODE_D]) {
            deltaX += 1;
        }

        Vector2Float delta = Vector2Float(deltaX, deltaY);

        delta.normalize();

        delta = (delta * (speed * deltaTime * Vectors::getFactor())).rotate(-getAngle());

        Vector2Float newPosFloat = Vectors::toVector2Float(position) + delta;
        Vector2Int newPos = Vectors::toVector2Int(newPosFloat);

        position = newPos;
    }
};