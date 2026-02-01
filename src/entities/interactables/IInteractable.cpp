//
// Created by timon on 1/18/26.
//

#include "IInteractable.h"

#include "entities/Humanoid.h"
#include "entities/scripts/Entity.h"
#include "math/Vectors.h"

#define MAX_INTERACTABLE_DISTANCE_PIXELS 128

std::string IInteractable::getInteractionText() const {
    return "[E]";
}

bool IInteractable::distance_check(const Entity * interactable, const Humanoid *activator) {
    float distance = (activator->getPosition() - interactable->getPosition()).length();
    float max_distance = Scaling::scaleToWorld(128.0f);

    if (distance > max_distance)
        return false;

    // auto raycast_hit = Physics::RayCast(activator->getPosition(),Vectors::toVector2Float(interactable->getPosition() - activator->getPosition()),spaceship,distance);
    // if (raycast_hit.hit)
    //     return false;

    return true;

    // return (interactable->getPosition() - activator->getPosition()).sqrLength() <= static_cast<float>(Scaling::scaleToWorld(MAX_INTERACTABLE_DISTANCE_PIXELS));
}
