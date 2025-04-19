#pragma once
#include "Entity.h"
#include "Vectors.h"
#include "Rendering.h"
#include <SDL3/SDL.h>

class BoxEntity : public Entity {
protected:
    Vector2Int scale; // Width and height in fixed-point format

public:
    BoxEntity(Vector2Int position, Vector2Int scale, float angle = 0.0f)
        : Entity(position,angle), scale(scale) {}

    virtual ~BoxEntity() {}

    Vector2Int getScale() const { return scale; }

    void setScale(const Vector2Int& s) { scale = s; }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override {
        // Get center on screen (same as in render)
        Vector2Float center = Vectors::toVector2Float(Vector2<int>::toScreenPosition(position - context.cameraPos)) / context.cameraScale;
        Vector2Float halfSize = Vectors::toVector2Float(scale) * 0.5f / context.cameraScale;

        // Local-space corners relative to the center
        Vector2Float corners[4] = {
            {-halfSize.x, -halfSize.y},
            { halfSize.x, -halfSize.y},
            { halfSize.x,  halfSize.y},
            {-halfSize.x,  halfSize.y}
        };

        // Rotate each corner around center
        float angleRad = getAngle() * (3.14159265f / 180.0f);
        float cosA = cos(angleRad);
        float sinA = sin(angleRad);

        for (int i = 0; i < 4; ++i) {
            float x = corners[i].x;
            float y = corners[i].y;
            corners[i].x = x * cosA - y * sinA + center.x;
            corners[i].y = x * sinA + y * cosA + center.y;
        }

        // Set debug color (white, or use something like red if you prefer)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        // Draw lines between corners
        for (int i = 0; i < 4; ++i) {
            int next = (i + 1) % 4;
            SDL_RenderLine(renderer,
                corners[i].x, corners[i].y,
                corners[next].x, corners[next].y);
        }
    }

};
