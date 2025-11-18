#pragma once
#include "Entity.h"

class RoundEntity : virtual public Entity {
protected:
    float radius;

public:
    RoundEntity(Vector2Int position,std::optional<float> angle, float radius,Behavior * behavior = nullptr)
        : Entity(position,angle,behavior), radius(radius) {}

    virtual ~RoundEntity() {}

    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }

    void debugRender(SDL_Renderer* renderer, const RenderingContext& context) override {

        Vector2Float center = context.toScreenPoint(position);

        float scaledRadius = radius / context.cameraScale;

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