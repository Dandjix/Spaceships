#pragma once
#include "Entity.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"
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
		// Set debug color (blue)
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        DebugRendering::drawWorldRect(renderer, context, position, scale, getAngle());
    }
};
