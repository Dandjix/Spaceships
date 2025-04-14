#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>
#include "Vector2.cpp"

class Player : public RoundEntity {
private:
    float speed;

public:
    Player(float x, float y, float speed)
        : RoundEntity(x, y, 20.0f), speed(speed) {}

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

        Vector2 delta = Vector2(deltaX, deltaY);

        delta.normalize();

        delta = delta *(speed * deltaTime);

        position = position + delta;
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                float dx = radius - w;
                float dy = radius - h;
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderPoint(renderer, (position.x + dx), (position.y + dy));
                }
            }
        }
    }
};
