#pragma once
#include "Entity.h"

class RoundEntity : public Entity {
protected:
    float radius;

public:
    RoundEntity(float x, float y, float radius)
        : Entity(x, y), radius(radius) {}

    virtual ~RoundEntity() {}

    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }
};