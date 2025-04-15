#pragma once
#include "BoxEntity.h"
#include <SDL3/SDL.h>
#include "Vectors.h"
#include "Vector2Float.h"
#include "Vector2Int.h"

class CargoContainer : public BoxEntity {
public:
    CargoContainer(int x, int y, int w, int h, float angle = 0.0f)
        : BoxEntity(x, y, w, h, angle) {}

    void update(float deltaTime) override {
        // CargoContainer might not need to move, so just leave it empty.
    }

    void render(SDL_Renderer* renderer) override {
        Vector2Float pos = toVector2Float(position);
        Vector2Float size = toVector2Float(scale);

        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown-ish color

        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        // Render rectangle manually with rotation
        for (int x = -scale.x / 2; x < scale.x / 2; ++x) {
            for (int y = -scale.y / 2; y < scale.y / 2; ++y) {
                // Apply rotation
                float rotatedX = x * cosA - y * sinA;
                float rotatedY = x * sinA + y * cosA;

                SDL_RenderPoint(renderer,
                    pos.x + rotatedX,
                    pos.y + rotatedY
                );
            }
        }
    }
};