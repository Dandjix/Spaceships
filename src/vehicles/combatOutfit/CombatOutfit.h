#pragma once
#include "../../vehicles/Vehicle.h"

class CombatOutfit : public Vehicle
{
    protected:

public:
    CombatOutfit(const Vector2Int &position, float angle,Humanoid *pilot = nullptr)
        : Vehicle(position, angle,new RoundPhysicsShape(this,Scaling::scaleToWorld(64.0f)), pilot)
    {

    }
public:
    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    nlohmann::json toJson() override;

    static Entity * fromJson(nlohmann::json::const_reference json, GameState::transientGameState &transient_game_state);

    constexpr std::string getJsonType() override;

    [[nodiscard]] std::string getVehicleName() const override {return "Combat Outfit";}

    Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

    Entity * finalizeRendering(const EntityRendering::Context &context) override {return this;}
};

