//
// Created by timon on 1/18/26.
//

#include "IInteractable.h"

#define MAX_INTERACTABLE_DISTANCE_PIXELS 128

bool IInteractable::distance_check(Entity * interactable,Humanoid *activator) {
    return (interactable->getPosition() - activator->getPosition()).sqrLength() <= Scaling::scaleToWorld(MAX_INTERACTABLE_DISTANCE_PIXELS);
}
