// Player.h
#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>

class Player : public RoundEntity {
private:
    float speed;

public:
    Player(float x, float y, float radius, float speed)
        : RoundEntity(x, y, radius), speed(speed) {}

    void update(float deltaTime) override {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_W]) {
            y -= speed * deltaTime;
        }
        if (state[SDL_SCANCODE_S]) {
            y += speed * deltaTime;
        }
        if (state[SDL_SCANCODE_A]) {
            x -= speed * deltaTime;
        }
        if (state[SDL_SCANCODE_D]) {
            x += speed * deltaTime;
        }
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderPoint(renderer, static_cast<int>(x + dx), static_cast<int>(y + dy));
                }
            }
        }
    }
};
