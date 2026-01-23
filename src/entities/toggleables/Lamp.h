//
// Created by timon on 1/23/26.
//

#pragma once
#include "entities/entityId/IdentityId.h"
#include "entities/toggleables/scripts/Toggleable.h"

class Lamp : public Toggleable{
protected:
    bool on;
public:
    Lamp(Vector2Int position, float angle, EntityId::entityId entity_id,bool on = true)
        : Toggleable(position, angle, entity_id), on(on) {
    }

    std::string getJsonType() override {return "lamp";}

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    bool getStatus() override {return on;}

    void setStatus(bool value) override {on = value;}

    nlohmann::json toJson() override;

    static Lamp * fromJson(nlohmann::json json, GameState::transientGameState & transient_game_state);
};
