//
// Created by timon on 12/3/25.
//

#include "PilotSeat.h"

#include "physics/shapes/RoundStaticPhysicsShape.h"

PilotSeat::PilotSeat(const Vector2Int &position, float angle, Humanoid *pilot) : Vehicle(
    position, angle, new RoundStaticPhysicsShape(this, Scaling::scaleToWorld(48.0f)), pilot) {
}

void PilotSeat::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

FROM_JSON_DEFINITION(PilotSeat) {
    if (json.contains("pilot")) {
        return new PilotSeat(Vector2Int::fromJson(json["position"]), json["angle"],
                             Humanoid::fromJson(json["pilot"], transient_game_state));
    }
    return new PilotSeat(Vector2Int::fromJson(json["position"]), json["angle"]);
}

void PilotSeat::update(const UpdateContext &context) {
    if (pilot == nullptr)
        return;

    if (pilot->getBehavior() == nullptr)
        return;

    pilot->getBehavior()->spaceshipControlUpdate(context, pilot, spaceship);
}

void PilotSeat::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (pilot == nullptr)
        return;

    if (pilot->getBehavior() == nullptr)
        return;

    pilot->getBehavior()->spaceshipControlHandleEvent(event, context, pilot, spaceship);
}
