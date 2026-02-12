#pragma once
#include "vehicles/Vehicle.h"

class CombatOutfit : public Vehicle
{
    protected:

public:
    CombatOutfit(const Vector2Int &position, float angle,Humanoid *pilot = nullptr);

public:
    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    nlohmann::json toJson() override;

    FROM_JSON_DECLARATION(CombatOutfit,"combat_outfit");

    [[nodiscard]] std::string getVehicleName() const override {return "Combat Outfit";}

    Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

    Entity * finalizeRendering(const EntityRendering::Context &context) override {return this;}
};

