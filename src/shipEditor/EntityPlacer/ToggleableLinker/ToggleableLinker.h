//
// Created by timon on 1/25/26.
//

#pragma once
#include "entities/scripts/ShortLivedEntity.h"


class ToggleableActivator;
class Toggleable;

class ToggleableLinker : public ShortLivedEntity {
public:
    enum State {
        Disabled,
        PickingActivator,
        PickingToggleable,
    };

protected:
    State state;

    std::vector<Entity *> *entities;

    Toggleable *closest_toggleable = nullptr;
    ToggleableActivator *closest_activator = nullptr;

    Toggleable *toggleable_to_link_to = nullptr;
    ToggleableActivator *activator_to_link_to = nullptr;

    Toggleable *getClosestToggleable(Vector2Int world_mouse_position) const;

    ToggleableActivator *getClosestToggleableActivator(Vector2Int world_mouse_position) const;

    static void drawLineToExisting(SDL_Renderer *renderer, const RenderingContext &context, ToggleableActivator *toggleable_activator);

public:
    ToggleableLinker(State state, std::vector<Entity *> *entities)
        : ShortLivedEntity({0, 0}, 0),
          state(state), entities(entities) {
    }

    void setState(State value);

    [[nodiscard]] State getState() const {
        return state;
    }


    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void update(const UpdateContext &context) override;

    void done(ToggleableActivator *toggleable_activator, Toggleable *toggleable);

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    QueueOrder::Value getQueueOrder() override;

    Entity * initializeRendering(const EntityRendering::Context &context) override {return this;}

    Entity * finalizeRendering(const EntityRendering::Context &context) override {return this;}
};
