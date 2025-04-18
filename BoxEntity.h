#pragma once
#include "Entity.h"
#include "Vectors.h"
#include "Rendering.h"

class BoxEntity : public Entity {
protected:
    Vector2Int scale; // Width and height in fixed-point format

public:
    BoxEntity(Vector2Int position, Vector2Int scale, float angle = 0.0f)
        : Entity(position,angle), scale(scale) {}

    virtual ~BoxEntity() {}

    Vector2Int getScale() const { return scale; }

    void setScale(const Vector2Int& s) { scale = s; }

    // These are still abstract unless you implement them here or in derived classes
    virtual void update(float deltaTime) override = 0;
    virtual void render(SDL_Renderer* renderer, RenderingContext context) override = 0;
};
