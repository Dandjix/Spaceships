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
};