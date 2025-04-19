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
        // Convert object position and camera to float
        Vector2Float floatPosition = Vectors::toVector2Float(position);
        Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);
        Vector2Float worldCenter = floatPosition - floatCameraPosition;
        Vector2Float center = worldCenter / context.cameraScale;
        center = Vector2Float::toScreenPosition(center);

        // Apply camera rotation
        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2.0f;
        Vector2Float diff = (screenCenter - center).rotate(context.cameraAngle);
        center = screenCenter - diff;

        // Calculate half size with camera scale
        Vector2Float halfSize = Vectors::toVector2Float(scale) * 0.5f / context.cameraScale;

        // Define corners in local space
        Vector2Float corners[4] = {
            {-halfSize.x, -halfSize.y},
            { halfSize.x, -halfSize.y},
            { halfSize.x,  halfSize.y},
            {-halfSize.x,  halfSize.y}
        };

        // Total rotation = object angle + camera angle
        float totalAngleDeg = getAngle() + context.cameraAngle;
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

        // Set debug color (blue)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        // Draw rectangle from corners
        for (int i = 0; i < 4; ++i) {
            int next = (i + 1) % 4;
            SDL_RenderLine(renderer,
                corners[i].x, corners[i].y,
                corners[next].x, corners[next].y);
        }
    }


};
