//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"

void Humanoid::update(const UpdateContext &context) {
    PhysicsEntity::update(context);
}

void Humanoid::handleEvent(const SDL_Event &event) {
    PhysicsEntity::handleEvent(event);
}

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {

    renderTexture(renderer,context,texture,Vector2Float(
        radius,
        radius
        )
    );
}

void Humanoid::movePosition(Vector2Float delta, SpaceShip * space_ship)
{
    Entity::movePosition(delta, space_ship);
}
