//
// Created by timon on 11/29/25.
//

#include "CombatOutfit.h"


void CombatOutfit::assumeControl(Humanoid *new_pilot) {

}

void CombatOutfit::relinquishControl() {
}

nlohmann::json CombatOutfit::toJson() {
    return Vehicle::toJson();
}

Entity * CombatOutfit::fromJson(nlohmann::json::const_reference json) {

    if (json.contains("pilot")) {
        return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],Humanoid::fromJson(json["pilot"]));
    }
    return new CombatOutfit(Vector2Int::fromJson(json["position"]),json["angle"],nullptr);
}

constexpr std::string CombatOutfit::getJsonType() {
    return "combat_outfit";
}


void CombatOutfit::render(SDL_Renderer *renderer, const RenderingContext &context) {

}
