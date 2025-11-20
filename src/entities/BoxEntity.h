#pragma once
#include "Entity.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"
#include <SDL3/SDL.h>

class BoxEntity : public Entity {
protected:
    Vector2Float scale; // Width and height in fixed-point format

public:
    BoxEntity(Vector2Int position, Vector2Float scale, std::optional<float> angle,Behavior * behavior)
        : Entity(position,angle,behavior), scale(scale) {}

    virtual ~BoxEntity() {}

    Vector2Float getScale() const { return scale; }

    void setScale(const Vector2Float& s) { scale = s; }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override {
		// Set debug color (blue)
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        // Calculate half size with camera scale
        Vector2Float halfSize = scale * 0.5f / context.cameraScale;

        Vector2Float center = context.toScreenPoint(position);

        // Define corners in local space
        Vector2Float corners[4] = {
            {-halfSize.x, -halfSize.y},
            { halfSize.x, -halfSize.y},
            { halfSize.x,  halfSize.y},
            {-halfSize.x,  halfSize.y}
        };

        // Total rotation = object angle + camera angle
        float totalAngleDeg = angle.value_or(0) + context.cameraAngle;
        float angleRad = totalAngleDeg * (3.14159265f / 180.0f);
        float cosA = cos(angleRad);
        float sinA = sin(angleRad);

        // Rotate corners around center
        for (int i = 0; i < 4; ++i) {
            float x = corners[i].x;
            float y = corners[i].y;
            corners[i].x = x * cosA - y * sinA + center.x;
            corners[i].y = x * sinA + y * cosA + center.y;
        }

        // Draw rectangle from corners
        for (int i = 0; i < 4; ++i) {
            int next = (i + 1) % 4;
            SDL_RenderLine(renderer,
                corners[i].x, corners[i].y,
                corners[next].x, corners[next].y);
        }

    }
};
