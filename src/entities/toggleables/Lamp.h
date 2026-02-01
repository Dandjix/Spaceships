//
// Created by timon on 1/23/26.
//

#pragma once
#include "entities/entityId/EntityId.h"
#include "entities/toggleables/scripts/Toggleable.h"
#include "spaceships/EntityData/EntityLoading.h"

class Lamp : public Toggleable{
protected:
    bool on;
public:
    Lamp(Vector2Int position, float angle, EntityId::entityId entity_id,bool on = true)
        : Toggleable(position, angle, entity_id), on(on) {
    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    bool getStatus() override {return on;}

    void setStatus(bool value) override {on = value;}

    nlohmann::json toJson() override;

    FROM_JSON_DECLARATION(Lamp,"lamp");

    Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

    Entity *finalizeRendering(const EntityRendering::Context &context) override {return this;}
};
