//
// Created by timon on 11/28/25.
//

#pragma once
#include "../physics/scripts/PhysicsEntity.h"
#include "behavior/Behavior.h"

class BehavioredEntity : public PhysicsEntity {

public:
    BehavioredEntity(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape)
        : PhysicsEntity(position, angle, shape) {
    }

    /**
     * @return true if the entity is the player, or a vehicle manned by the player
     */
    virtual constexpr bool is_player();

    virtual void setBehavior(Behavior * value)= 0;

    virtual Behavior * getBehavior() const = 0;

    void update(const UpdateContext &context) override {
        if (getBehavior() != nullptr) {
            getBehavior()->update(context,this);
        }
    }

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override {
        if (getBehavior() != nullptr) {
            getBehavior()->handleEvent(event,context,this);
        }
    }
};

constexpr bool BehavioredEntity::is_player() {
    if (getBehavior() ==nullptr)
        return false;
    return getBehavior()->isPlayerBehavior();
}
