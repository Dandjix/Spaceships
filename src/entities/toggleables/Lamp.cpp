//
// Created by timon on 1/23/26.
//

#include "Lamp.h"

#include "game/Rendering.h"
#include "loadGame/GameState.h"

void Lamp::render(SDL_Renderer *renderer, const RenderingContext &context) {

    Vector2Float screenPos = context.camera_info.worldToScreenPoint(position);

    SDL_SetRenderDrawColor(renderer,255,255,0,255);
    // SDL_RenderDrawLine

    SDL_RenderLine(renderer,screenPos.x-64,screenPos.y+64,screenPos.x+64,screenPos.y-64);


    //light
    if (!on)
        return;
    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(renderer,&original_blend_mode);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    SDL_RenderLine(renderer,screenPos.x-64,screenPos.y-64,screenPos.x+64,screenPos.y+64);

}

nlohmann::json Lamp::toJson() {
    auto json = Entity::toJson();
    json["entity_id"] = entity_id;
    json["on"] = on;
    return json;
}

FROM_JSON_DEFINITION(Lamp) {
    auto lamp = new Lamp(Vector2Int::fromJson(json["position"]),json["angle"],json["entity_id"],json["on"]);
    transient_game_state.identified_entities[json["entity_id"]] = lamp;
    return lamp;
}
