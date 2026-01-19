#pragma once
#include "../entities/scripts/Entity.h"
#include "../entities/Humanoid.h"
#include "../spaceships/SpaceShip.h"
#include "entities/interactables/IInteractable.h"

class Vehicle : public BehavioredEntity, IInteractable {
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

protected:
    void assumeControl(Humanoid * new_pilot);

    void relinquishControl(Humanoid * old_pilot);

public:
    void onRegistered(SpaceShip *newSpaceship) override;

    Humanoid *getPilot() {
        return pilot;
    }

    void interact(Humanoid *activator) override;

    bool is_interactable(Humanoid *activator) override;

    void stopPiloting();
    virtual bool canStopPiloting();

    nlohmann::json toJson() override = 0;

    constexpr bool is_player() override;

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    void setBehavior(Behavior *value) override;

    Behavior *getBehavior() const override;

    [[nodiscard]] PhysicsEntity * asEntity() override {return this;}

    [[nodiscard]] IInteractable * asIInteractable() override {return this;}

    [[nodiscard]] virtual std::string getVehicleName() const = 0;

    [[nodiscard]] std::string getInteractionText() const override;
};
