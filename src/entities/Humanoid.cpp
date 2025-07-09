//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

void Humanoid::update(const UpdateContext &context) {
    RoundEntity::update(context);
}

void Humanoid::handleEvent(const SDL_Event &event) {
    RoundEntity::handleEvent(event);
}

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {
    Vector2Int cameraPosition = (position - context.cameraPos).scaleToScreenPosition()/context.cameraScale;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    float scaledRadius = radius / context.cameraScale;
    for (int w = 0; w < scaledRadius * 2; w++) {
        for (int h = 0; h < scaledRadius * 2; h++) {
            float dx = scaledRadius - w;
            float dy = scaledRadius - h;
            if (dx * dx + dy * dy <= scaledRadius * scaledRadius) {
                SDL_RenderPoint(renderer, static_cast<float>(cameraPosition.x) + dx, static_cast<float>(cameraPosition.y) + dy);
            }
        }
    }
}
