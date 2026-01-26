//
// Created by timon on 1/25/26.
//

#pragma once
#include "physics/PhysicsEntity.h"

class Toggleable;

class ToggleableActivator{
public:
    virtual ~ToggleableActivator() = default;

    virtual void setLinkedEntity(Toggleable * entity_to_link) = 0;

    [[nodiscard]] virtual Toggleable * getLinkedEntity() = 0;

    [[nodiscard]] virtual Entity * asEntity()=0;
};
