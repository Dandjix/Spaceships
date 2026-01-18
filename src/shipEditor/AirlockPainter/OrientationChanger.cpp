//
// Created by timon on 1/18/26.
//

#include "OrientationChanger.h"

void AirlockPainter::OrientationChanger::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type != SDL_EVENT_MOUSE_WHEEL) return;
    if (!(SDL_GetModState() & SDL_KMOD_ALT)) return;

    Airlock::Orientation current = painter->getOrientation();

    std::vector<Airlock::Orientation> orientations = {
        Airlock::Orientation::North,
        Airlock::Orientation::East,
        Airlock::Orientation::South,
        Airlock::Orientation::West,
    };

    auto it = std::ranges::find(orientations, current);
    if (it == orientations.end()) return; // safety check
    int index = std::distance(orientations.begin(), it);

    if (event.wheel.y > 0) index+= 1;
    else index -= 1;

    index = index % orientations.size();
    if(index < 0) index += orientations.size();

    painter->setOrientation(orientations[index]);
}
