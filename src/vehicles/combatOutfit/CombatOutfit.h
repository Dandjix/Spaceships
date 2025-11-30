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

protected:
    void assumeControl(Humanoid * pilot) override;

    void relinquishControl() override;

public:
    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    nlohmann::json toJson() override;

    static Entity * fromJson(nlohmann::json::const_reference json);

    constexpr std::string getJsonType() override;

    std::string getVehicleName() override {return "Combat Outfit";}
};

