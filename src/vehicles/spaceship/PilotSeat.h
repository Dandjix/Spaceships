//
// Created by timon on 12/3/25.
//

#pragma once
#include "vehicles/Vehicle.h"
#include "physics/shapes/RoundStaticPhysicsShape.h"

class PilotSeat : public Vehicle {
public:
    PilotSeat(const Vector2Int &position, float angle, Humanoid *pilot = nullptr)
        : Vehicle(position, angle, new RoundStaticPhysicsShape(this, Scaling::scaleToWorld(48.0f)), pilot) {
    }

    ~PilotSeat() override = default;


    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    [[nodiscard]] std::string getVehicleName() const override {return "Pilot Seat";}

public:
    constexpr std::string getJsonType() override { return "pilot_seat";}
    nlohmann::json toJson() override {return Vehicle::toJson();}
    static PilotSeat * fromJson(nlohmann::json json, GameState::transientGameState &transient_game_state) {
        if (json.contains("pilot")) {
            return new PilotSeat(Vector2Int::fromJson(json["position"]),json["angle"],Humanoid::fromJson(json["pilot"], transient_game_state));
        }
        return new PilotSeat(Vector2Int::fromJson(json["position"]),json["angle"]);
    }

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    constexpr Behavior::Control getControlType() override {return Behavior::PilotingSpaceship;}

    Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

    Entity *finalizeRendering(const EntityRendering::Context &context) override {return this;}
};
