//
// Created by timon on 1/23/26.
//

#pragma once
#include "entities/entityId/EntityId.h"
#include "entities/toggleables/scripts/IToggleable.h"
#include "spaceships/EntityData/EntityLoading.h"

class Lamp : public Entity, public virtual IToggleable {
protected:
    EntityId::entityId entity_id;
    bool on;

public:
    Lamp(Vector2Int position, float angle, EntityId::entityId entity_id, bool on = true)
        : Entity(position, angle), on(on), entity_id(entity_id) {
    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    bool getStatus() override { return on; }

    void setStatus(bool value) override { on = value; }

    nlohmann::json toJson() override;

    FROM_JSON_DECLARATION(Lamp, "lamp");

    Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

    Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }

    [[nodiscard]] EntityId::entityId getEntityId() const override { return entity_id; }

    [[nodiscard]] Entity *asEntity() override { return this; }

    void toggle() override { on = !on; }

    [[nodiscard]] IToggleable * asToggleable() override {return this;}
};
