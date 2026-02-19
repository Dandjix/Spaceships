//
// Created by timon on 1/25/26.
//

#pragma once
#include "../../../physics/scripts/PhysicsEntity.h"

class IToggleable;

class ToggleableActivator{
public:
    virtual ~ToggleableActivator() = default;

    virtual void setLinkedEntity(IToggleable * entity_to_link) = 0;

    [[nodiscard]] virtual IToggleable * getLinkedEntity() = 0;

    [[nodiscard]] virtual Entity * asEntity()=0;
};
