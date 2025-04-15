#pragma once
#include "RoundEntity.h"
#include <SDL3/SDL.h>
#include "Vector2Int.h"
#include "Vector2Float.h"
#include "Vectors.h"

class Player : public RoundEntity {
private:
    float speed;

public:
    Player(int x, int y, float speed)
        : RoundEntity(x, y, 20), speed(speed) {}

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

        Vector2Float newPosFloat = toVector2Float(position) + delta;
        Vector2Int newPos = toVector2Int(newPosFloat);

        position = newPos;
    }

    void render(SDL_Renderer* renderer, const Vector2Float cameraPos, int screenWidth, int screenHeight) {
        Vector2Float cameraPosition = toVector2Float(position);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
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
