#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>
#include "Vectors.h"
#include "Rendering.h"

class Player : public RoundEntity {
private:
    float speed;

public:
    Player(Vector2Int position,float angle, float speed)
        : RoundEntity(position,angle, 20), speed(speed) {}

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

        delta = delta *(speed * deltaTime);

        Vector2Float newPosFloat = Vectors::toVector2Float(position) + delta;
        Vector2Int newPos = Vectors::toVector2Int(newPosFloat);

        position = newPos;
    }

    void render(SDL_Renderer* renderer, const RenderingContext& context) {
        Vector2Int cameraPosition = Vectors::toScreenPosition(position - context.cameraPos);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderPoint(renderer, (cameraPosition.x + dx), (cameraPosition.y + dy));
                }
            }
        }
    }
};
