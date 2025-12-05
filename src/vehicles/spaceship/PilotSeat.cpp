//
// Created by timon on 12/3/25.
//

#include "PilotSeat.h"

void PilotSeat::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

void PilotSeat::update(const UpdateContext &context) {
    if (pilot == nullptr)
        return;

    if (pilot->getBehavior()==nullptr)
        return;

    pilot->getBehavior()->spaceshipControlUpdate(context,pilot,spaceship);
}

void PilotSeat::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (pilot == nullptr)
        return;

    if (pilot->getBehavior()==nullptr)
        return;

    pilot->getBehavior()->spaceshipControlHandleEvent(event,context,pilot,spaceship);
}
