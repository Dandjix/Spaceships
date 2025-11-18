//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"

void Humanoid::update(const UpdateContext &context) {
    RoundEntity::update(context);
}

void Humanoid::handleEvent(const SDL_Event &event) {
    RoundEntity::handleEvent(event);
}

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {

    renderTexture(renderer,context,texture,Vector2Float(
        radius,
        radius
        )
    );
}

const float cushion = 0.1f; // 1/10th of a pixel

void Humanoid::movePosition(Vector2Float delta, SpaceShip * space_ship)
{
    Entity::movePosition(delta, space_ship);
}
