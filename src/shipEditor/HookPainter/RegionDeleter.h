//
// Created by timon on 11/28/25.
//

#pragma once
#include "entities/Entity.h"
#include "entities/ShortLivedEntity.h"

namespace HookDeleter{

    class RegionDeleter : public ShortLivedEntity{
    private:
        SpaceshipHooks * hooks;
        bool enabled;

        HookRegion * closest_region;

        std::string closest_name;

        void renderRegion(SDL_Renderer * renderer, const RenderingContext & context) const;

        void deleteRegion(const std::string &region_name);

    public:
        explicit RegionDeleter(SpaceshipHooks * hooks, bool enabled = false)
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

        unsigned short getQueueOrder() override {return 30000;}
    };
}
