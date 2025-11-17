#include "PlayerBehavior.h"
#include "../entities/Humanoid.h"

void PlayerBehavior::update(const UpdateContext &context, Entity * owner) {
    const bool * state = SDL_GetKeyboardState(nullptr);
    float deltaX = 0;
    float deltaY = 0;
    if (state[SDL_SCANCODE_W]) {
        deltaY -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        deltaY += 1;
    }
    if (state[SDL_SCANCODE_A]) {
        deltaX -= 1;
    }
    if (state[SDL_SCANCODE_D]) {
        deltaX += 1;
    }

    Vector2Float delta = Vector2Float(deltaX, deltaY).rotate(-camera->getAngle());

    delta.normalize();

    delta = delta * context.deltaTime * 600; //TODO : remove * 600

    delta = delta.scaleToWorldPosition();

    owner->movePosition(delta,context.spaceShip);
}

void PlayerBehavior::handleEvent(const SDL_Event &event, Entity *owner) {
}
