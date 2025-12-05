#pragma once
#include "../entities/scripts/Entity.h"
#include "../entities/Humanoid.h"
#include "../spaceships/SpaceShip.h"

class Vehicle : public BehavioredEntity {
protected:
    Humanoid *pilot;

    SpaceShip *spaceship = nullptr;

public:
    Vehicle(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape,
            Humanoid *pilot = nullptr)
        : BehavioredEntity(position, angle, shape),
          pilot(pilot) {
    }

    ~Vehicle() override {
        delete pilot;
    }

    virtual constexpr Behavior::Control getControlType(){ return Behavior::PilotingVehicle;}

    virtual std::string getVehicleName() =0;

protected:
    void assumeControl(Humanoid * new_pilot);

    void relinquishControl(Humanoid * old_pilot);

public:
    void onRegistered(SpaceShip *newSpaceship) override;

    Humanoid *getPilot() {
        return pilot;
    }

    void startPiloting(Humanoid *newPilot);

    void stopPiloting();

    virtual bool canStartPiloting(Humanoid *newPilot);

    virtual bool canStopPiloting();

    nlohmann::json toJson() override = 0;

    constexpr bool is_player() override;

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    void setBehavior(Behavior *value) override;

    Behavior *getBehavior() const override;
};
