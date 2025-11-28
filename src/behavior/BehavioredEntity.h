//
// Created by timon on 11/28/25.
//

#pragma once
#include "physics/PhysicsEntity.h"

class BehavioredEntity : public PhysicsEntity {
protected:
    Behavior * behavior;

public:
    BehavioredEntity(const Vector2Int &position, const std::optional<float> &angle, PhysicsShape *shape, Behavior * behavior)
        : PhysicsEntity(position, angle, shape),behavior(behavior) {
    }

    void setBehavior(Behavior * value) {
        behavior = value;
    }

    Behavior * getBehavior() {
        return behavior;
    }

    void update(const UpdateContext &context) override {
        if (behavior != nullptr) {
            behavior->update(context,this);
        }
    }

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override {
        if (behavior != nullptr) {
            behavior->handleEvent(event,context,this);
        }
    }
};
