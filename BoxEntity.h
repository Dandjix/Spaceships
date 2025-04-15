#pragma once
#include "Entity.h"
#include "Vector2Int.h"

class BoxEntity : public Entity {
protected:
    Vector2Int scale; // Width and height in fixed-point format
    float angle;      // Rotation in radians

public:
    BoxEntity(int x, int y, int w, int h, float angle = 0.0f)
        : Entity(Vector2Int(x, y)), scale(Vector2Int(w, h)), angle(angle) {}

    BoxEntity(Vector2Int position, Vector2Int scale, float angle = 0.0f)
        : Entity(position), scale(scale), angle(angle) {}

    virtual ~BoxEntity() {}

    Vector2Int getScale() const { return scale; }
    float getAngle() const { return angle; }

    void setScale(const Vector2Int& s) { scale = s; }
    void setAngle(float a) { angle = a; }

    // These are still abstract unless you implement them here or in derived classes
    virtual void update(float deltaTime) override = 0;
    virtual void render(SDL_Renderer* renderer, const Vector2Float cameraPos, int screenWidth, int screenHeight) override = 0;
};
