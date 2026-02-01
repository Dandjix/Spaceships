//
// Created by timon on 11/29/25.
//

#include "CombatOutfit.h"

#include "physics/shapes/RoundPhysicsShape.h"


nlohmann::json CombatOutfit::toJson() {
    return Vehicle::toJson();
}

FROM_JSON_DEFINITION(CombatOutfit) {
    if (json.contains("pilot")) {
        return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],Humanoid::fromJson(json["pilot"], transient_game_state));
    }
    return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],nullptr);
}


CombatOutfit::CombatOutfit(const Vector2Int &position, float angle, Humanoid *pilot): Vehicle(position, angle,new RoundPhysicsShape(this,Scaling::scaleToWorld(64.0f)), pilot) {

}

void CombatOutfit::render(SDL_Renderer *renderer, const RenderingContext &context) {

}
