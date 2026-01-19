//
// Created by timon on 1/18/26.
//

#include "IInteractable.h"

#define MAX_INTERACTABLE_DISTANCE_PIXELS 128

std::string IInteractable::getInteractionText() const {
    return "[E]";
}

bool IInteractable::distance_check(const Entity * interactable, const Humanoid *activator) {
    return (interactable->getPosition() - activator->getPosition()).sqrLength() <= static_cast<float>(Scaling::scaleToWorld(MAX_INTERACTABLE_DISTANCE_PIXELS));
}
