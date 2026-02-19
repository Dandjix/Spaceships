//
// Created by timon on 1/20/26.

#pragma once
#include "entities/scripts/Entity.h"
#include "entities/entityId/IIdentifiedEntity.h"


/**
 * Abstraction that represents any device which can be controlled by another (airlock; door; hell, even a turret since it
 * could be turned on or off)
 */
class IToggleable : public virtual IIdentifiedEntity {
public:
    /**
     * Toggles the object. If it is a lamp for instance, cycling it twice could turn it on then back off.
     */
    virtual void toggle() = 0;
    virtual bool getStatus() = 0;
    virtual void setStatus(bool value) = 0;
};
