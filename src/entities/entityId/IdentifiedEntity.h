//
// Created by timon on 1/20/26.
//

#pragma once
#include <cstdint>


class Humanoid;
class Vehicle;
class Toggleable;
class Entity;

class IdentifiedEntity {

public:
    virtual ~IdentifiedEntity() = default;

    virtual void makeReal()=0;

    [[nodiscard]] virtual Entity * asEntity() = 0;

    //more interpretations to be added as necessary

    [[nodiscard]] virtual Humanoid * asHumanoid() {return nullptr;}
    [[nodiscard]] virtual Vehicle * asVehicle() {return nullptr;}
    [[nodiscard]] virtual Toggleable * asToggleable() {return nullptr;}
};
