#pragma once
#include "Entity.h"

class RoundEntity : public Entity {
protected:
    int radius;

public:
    RoundEntity(int x, int y, int radius)
        : Entity(x, y), radius(radius) {}  // Calling Entity(float x, float y)

    RoundEntity(Vector2Int position, int radius)
        : Entity(position), radius(radius) {}  // Calling Entity(Vector2 position)

    virtual ~RoundEntity() {}

    int getRadius() const { return radius; }
    void setRadius(int r) { radius = r; }
};