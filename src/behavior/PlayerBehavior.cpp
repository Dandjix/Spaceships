#include "PlayerBehavior.h"
#include "../entities/Humanoid.h"
#include "spaceships/SpaceShip.h"

void PlayerBehavior::update(const UpdateContext &context, PhysicsEntity * owner) {
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

    Vector2Float delta = Vector2Float(deltaX, deltaY).rotate(-context.camera_info.cameraAngle);

    delta.normalize();

    delta = delta * context.deltaTime * 200; //TODO : remove * 600

    delta = delta.scaleToWorldPosition();

    owner->movePosition(delta);
}

void PlayerBehavior::spaceshipControlUpdate(const UpdateContext &context, PhysicsEntity *owner, SpaceShip *space_ship) {
    const bool * state = SDL_GetKeyboardState(nullptr);
    float deltaX = 0;
    float deltaY = 0;
    float deltaAngle = 0;
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

    if (state[SDL_SCANCODE_J]) {
        deltaAngle +=1;
    }
    if (state[SDL_SCANCODE_K]) {
        deltaAngle -=1;
    }

    Vector2Float delta = Vector2Float(deltaX, deltaY).rotate(space_ship->getAngle());

    delta.normalize();

    delta = delta * context.deltaTime * 200; //TODO : remove * 600

    delta = delta.scaleToWorldPosition();

    space_ship->setPosition(space_ship->getPosition() + Vectors::toVector2Int(delta));
    space_ship->setAngleAroundCenter(space_ship->getAngle()+deltaAngle);
}

void PlayerBehavior::spaceshipControlHandleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context,
    PhysicsEntity *owner, SpaceShip *space_ship) {

    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_0) {
            std::cout << "Undocking" << std::endl;
        }
        if (event.key.key == SDLK_1) {
            std::cout << "Docking to 1" << std::endl;
        }
        if (event.key.key == SDLK_2) {
            std::cout << "Docking to 2" << std::endl;
        }
    }
}

void PlayerBehavior::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext & context, PhysicsEntity *owner) {
}

nlohmann::json PlayerBehavior::toJson() {
    return "player_behavior";
}
