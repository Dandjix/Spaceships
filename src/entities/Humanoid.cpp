//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"

void Humanoid::update(const UpdateContext &context) {
    if (behavior != nullptr)
        behavior->update(context,this);
}

void Humanoid::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (behavior != nullptr)
        behavior->handleEvent(event,context,this);
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
