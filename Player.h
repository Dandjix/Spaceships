#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>
#include "Vectors.h"
#include "Rendering.h"

class Player : public RoundEntity {
private:
    float speed;

    Camera* camera = nullptr;

public:
    Player(Vector2Int position,float angle, float speed, Camera * camera)
        : RoundEntity(position,angle, 20), speed(speed), camera(camera) {}

    void setCamera(Camera* c)
    {
        camera = c;
    }

    void update(float deltaTime) override {
        const bool * state = SDL_GetKeyboardState(NULL);
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

        delta = (delta *(speed * deltaTime * Vectors::getFactor())).rotate(-camera->getAngle());

        Vector2Float newPosFloat = Vectors::toVector2Float(position) + delta;
        Vector2Int newPos = Vectors::toVector2Int(newPosFloat);

        position = newPos;
    }

    void render(SDL_Renderer* renderer, const RenderingContext& context) override {
        Vector2Int cameraPosition = (position - context.cameraPos).scaleToScreenPosition()/context.cameraScale;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        float scaledRadius = radius / context.cameraScale;
        for (int w = 0; w < scaledRadius * 2; w++) {
            for (int h = 0; h < scaledRadius * 2; h++) {
                float dx = scaledRadius - w;
                float dy = scaledRadius - h;
                if (dx * dx + dy * dy <= scaledRadius * scaledRadius) {
                    SDL_RenderPoint(renderer, static_cast<float>(cameraPosition.x) + dx, static_cast<float>(cameraPosition.y) + dy);
                }
            }
        }
    }
};
