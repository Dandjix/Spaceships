//
// Created by timon on 1/18/26.

#pragma once
#include "entities/Humanoid.h"


/**
 * Interactable objects, such as buttons, vehicles, terminals ...
 */
class IInteractable {
public:
    virtual ~IInteractable() = default;

    static bool distance_check(Entity *interactable, Humanoid *activator);

    /**
     * This should do what the object is supposed to do when interacted with. This should not check whether the object is
     * interactable, this is the responsibility of the AI / player control
     * @param activator the entity interacting with this object
     */
    virtual void interact(Humanoid * activator) = 0;

    /**
     * This returns whether or not the object can be interacted with. It is most probable that this should call
     * IInteractable::distance_check
     * @param activator
     * @return
     */
    virtual bool is_interactable(Humanoid * activator) = 0;
};
