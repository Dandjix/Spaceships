#pragma once
#include "../entities/Entity.h"
#include "../entities/Humanoid.h"
#include "../spaceships/SpaceShip.h"

class Vehicle : public Entity
{
protected:
    Humanoid *pilot;

    SpaceShip *spaceship;

    virtual void assumeControl(Humanoid * pilot) = 0;
    virtual void reliquishControl(Humanoid * pilot) = 0;

public:
    
    void onRegistered(SpaceShip * newSpaceship) override
    {
        spaceship = newSpaceship;
    }

    Humanoid * getPilot()
    {
        return pilot;
    }

    void startPiloting(Humanoid *newPilot)
    {
        stopPiloting();

        pilot = newPilot;
        spaceship->registerEntities({pilot});
    }

    void stopPiloting()
    {
        if(pilot == nullptr) return;
        
        pilot->setPosition(getPosition());
        pilot->setAngle(getAngle());
        spaceship->unregisterEntities({pilot});

        pilot = nullptr;
    }
};