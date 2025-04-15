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
        Vector2Float center = toVector2Float(position);
        Vector2Float halfSize = toVector2Float(scale) * 0.5f;

        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Brown-ish color

        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        // Compute bounding box of rotated rectangle
        float maxX = std::ceil(std::abs(halfSize.x * cosA) + std::abs(halfSize.y * sinA));
        float maxY = std::ceil(std::abs(halfSize.x * sinA) + std::abs(halfSize.y * cosA));

        // Scan the bounding box
        for (int x = -maxX; x <= maxX; ++x) {
            for (int y = -maxY; y <= maxY; ++y) {
                // Unrotate point back into local space
                float localX = x * cosA + y * sinA;
                float localY = -x * sinA + y * cosA;

                // Check if the point is within the unrotated box
                if (std::abs(localX) <= halfSize.x && std::abs(localY) <= halfSize.y) {
                    SDL_RenderPoint(renderer, center.x + x, center.y + y);
                }
            }
        }
    }

};