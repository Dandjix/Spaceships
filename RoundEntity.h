#pragma once
#include "Entity.h"

class RoundEntity : public Entity {
protected:
    int radius;

public:
    RoundEntity(Vector2Int position,std::optional<float> angle, int radius)
        : Entity(position,angle), radius(radius) {}  // Calling Entity(Vector2 position)

    virtual ~RoundEntity() {}

    int getRadius() const { return radius; }
    void setRadius(int r) { radius = r; }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override {
        // Convert position and camera to float
        Vector2Float floatPosition = Vectors::toVector2Float(position);
        Vector2Float floatCameraPosition = Vectors::toVector2Float(context.cameraPos);

        // Convert world position to screen space
        Vector2Float worldCenter = floatPosition - floatCameraPosition;
        Vector2Float center = worldCenter / context.cameraScale;
        center = Vector2Float::toScreenPosition(center);

        // Adjust for camera rotation
        Vector2Float screenCenter = Vectors::toVector2Float(context.screenDimensions) / 2.0f;
        Vector2Float diff = (screenCenter - center).rotate(context.cameraAngle);
        center = screenCenter - diff;

        // Calculate scaled radius
        float scaledRadius = static_cast<float>(radius) / context.cameraScale;

        // Set debug draw color (green, for example)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        // Draw debug circle
        const int segments = 16; // More = smoother circle
        float angleStep = 2.0f * 3.14159265f / segments;

        for (int i = 0; i < segments; ++i) {
            float theta1 = i * angleStep;
            float theta2 = (i + 1) % segments * angleStep;

            float x1 = center.x + cosf(theta1) * scaledRadius;
            float y1 = center.y + sinf(theta1) * scaledRadius;
            float x2 = center.x + cosf(theta2) * scaledRadius;
            float y2 = center.y + sinf(theta2) * scaledRadius;

            SDL_RenderLine(renderer, x1, y1, x2, y2);
        }
    }

};