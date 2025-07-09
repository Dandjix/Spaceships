#pragma once
#include "../../vehicles/Vehicle.h"

class CombatOutfit : Vehicle
{
    protected:
    void assumeControl(Humanoid * pilot) override;

    void reliquishControl(Humanoid * pilot) override;
};