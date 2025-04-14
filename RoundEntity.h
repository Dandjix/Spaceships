#pragma once
#include "Entity.h"

class RoundEntity : public Entity {
protected:
    float radius;

public:
    RoundEntity(float x, float y, float radius)
        : Entity(x, y), radius(radius) {}  // Calling Entity(float x, float y)

    RoundEntity(Vector2 position, float radius)
        : Entity(position), radius(radius) {}  // Calling Entity(Vector2 position)

    virtual ~RoundEntity() {}

    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }
};