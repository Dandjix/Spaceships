//
// Created by timon on 12/19/25.
//

#pragma once
#include "entities/scripts/ShortLivedEntity.h"


class HookRegion;
class SpaceshipHooks;

class AirlockPainterManager : public ShortLivedEntity{
private:
    SpaceshipHooks * hooks;
    bool enabled;

    HookRegion * closest_region;

    std::string closest_name;

    void renderRegion(SDL_Renderer * renderer, const RenderingContext & context) const;

public:
    explicit AirlockPainterManager(SpaceshipHooks * hooks, bool enabled = false)
        : ShortLivedEntity({0,0}, 0),hooks(hooks), enabled(enabled), closest_region(nullptr){
    }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        if (!enabled)
            return;

        if (closest_region == nullptr)
            return;

        renderRegion(renderer,context);
    }

    void update(const UpdateContext &context) override;

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    void enable();

    void disable();

    QueueOrder::Value getQueueOrder() override {return QueueOrder::LAST;}
};
