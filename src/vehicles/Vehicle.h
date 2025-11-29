#pragma once
#include "../entities/Entity.h"
#include "../entities/Humanoid.h"
#include "../spaceships/SpaceShip.h"

class Vehicle : public PhysicsEntity
{
protected:
    Humanoid *pilot;

    SpaceShip *spaceship = nullptr;

public:
    Vehicle(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape,Humanoid *pilot = nullptr)
        : PhysicsEntity(position, angle, shape),
          pilot(pilot)
    {
    }

    ~Vehicle() override {
        delete pilot;
    }

protected:
    virtual void assumeControl(Humanoid * pilot) = 0;
    virtual void relinquishControl() = 0;

public:
    
    void onRegistered(SpaceShip * newSpaceship) override;

    Humanoid * getPilot()
    {
        return pilot;
    }

    void startPiloting(Humanoid *newPilot);

    void stopPiloting();

    nlohmann::json toJson() override;

    constexpr bool is_player() override;

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;
};



