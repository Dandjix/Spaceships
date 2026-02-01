//
// Created by timon on 11/29/25.
//

#include "CombatOutfit.h"

#include "physics/shapes/RoundPhysicsShape.h"


nlohmann::json CombatOutfit::toJson() {
    return Vehicle::toJson();
}

Entity * CombatOutfit::fromJson(nlohmann::json::const_reference json, GameState::transientGameState &transient_game_state) {

    if (json.contains("pilot")) {
        return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],Humanoid::fromJson(json["pilot"], transient_game_state));
    }
    return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],nullptr);
}

constexpr std::string CombatOutfit::getJsonType() {
    return "combat_outfit";
}


CombatOutfit::CombatOutfit(const Vector2Int &position, float angle, Humanoid *pilot): Vehicle(position, angle,new RoundPhysicsShape(this,Scaling::scaleToWorld(64.0f)), pilot) {

}

void CombatOutfit::render(SDL_Renderer *renderer, const RenderingContext &context) {

}
