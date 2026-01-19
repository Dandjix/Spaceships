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

    [[nodiscard]] virtual std::string getInteractionText() const;

    static bool distance_check(const Entity *interactable, const Humanoid *activator);

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

    /**
     * yeah so uhhh ...
     * An interactable is always a physics entity since it has to have a hitbox (or else you could never interact with it)
     *
     * @return this as an entity (this is always meant to be an entity, but diamond inheritance looks scary so this is an interface)
     * coulda used a dynamic cast to make this more obvious but then we'd run the risk of someone trying to make something
     * that is not an entity an interactable.
     *
     */
    [[nodiscard]] virtual PhysicsEntity *asEntity() = 0;
};