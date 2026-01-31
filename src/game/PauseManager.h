//
// Created by timon on 12/3/25.
//

#pragma once
#include "entities/scripts/ActiveWhenPausedEntity.h"


class PauseManager : public ActiveWhenPausedEntity {
public:

    bool * paused;

    Event<bool> on_paused_change;

    PauseManager(bool * paused)
        : ActiveWhenPausedEntity({0,0},0),paused(paused) {
    }

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void setPaused(bool cond);

    PauseManager * initializeRendering(const EntityRendering::Context &context) override{return this;}

    PauseManager * finalizeRendering(const EntityRendering::Context &context) override{return this;}
};
