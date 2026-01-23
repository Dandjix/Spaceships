//
// Created by timon on 1/20/26.

#pragma once
#include "entities/scripts/Entity.h"
#include "entities/entityId/IdentifiedEntity.h"


/**
 * Abstraction that represents any device which can be controlled by another (airlock; door; hell, even a turret since it
 * could be turned on or off)
 */
class Toggleable : public Entity, public virtual IdentifiedEntity {
public:
    const EntityId::entityId entity_id;

    Toggleable(Vector2Int position,  std::optional<float> angle, EntityId::entityId entity_id)
        : Entity(position, angle),entity_id(entity_id) {
    }

    /**
     * Toggles the object. If it is a lamp for instance, cycling it twice could turn it on then back off.
     */
    void toggle() {
        setStatus(!getStatus());
    }

    virtual bool getStatus() = 0;
    virtual void setStatus(bool value) = 0;

    [[nodiscard]] Entity *asEntity() override { return this; }
    [[nodiscard]] Toggleable *asToggleable() override { return this; }

    nlohmann::json toJson() override {
        nlohmann::json json = Entity::toJson();
        json["entity_id"] = entity_id;
        return json;
    }
};
