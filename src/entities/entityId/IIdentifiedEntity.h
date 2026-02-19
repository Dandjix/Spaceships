//
// Created by timon on 1/20/26.
//

#pragma once
#include <cstdint>

#include "EntityId.h"


class Humanoid;
class Vehicle;
class IToggleable;
class Entity;

class IIdentifiedEntity {

public:
    virtual ~IIdentifiedEntity() = default;

    [[nodiscard]] virtual EntityId::entityId getEntityId() const = 0;

    [[nodiscard]] virtual Entity * asEntity() = 0;

    //more interpretations to be added as necessary

    [[nodiscard]] virtual Humanoid * asHumanoid() {return nullptr;}
    [[nodiscard]] virtual Vehicle * asVehicle() {return nullptr;}
    [[nodiscard]] virtual IToggleable * asToggleable() {return nullptr;}
};
