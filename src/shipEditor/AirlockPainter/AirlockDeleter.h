//
// Created by timon on 11/28/25.
//

#pragma once
#include "../../entities/scripts/Entity.h"
#include "../../entities/scripts/ShortLivedEntity.h"

class Airlock;
class SpaceshipHooks;

namespace AirlockDeleter{

    class AirlockDeleter : public ShortLivedEntity{
    private:
        SpaceshipHooks * hooks;
        bool enabled;

        Airlock * closest_airlock;

        std::string closest_name;

        void renderAirlock(SDL_Renderer * renderer, const RenderingContext & context) const;

        void deleteAirlock(const std::string &airlock_name);

    public:
        explicit AirlockDeleter(SpaceshipHooks * hooks, bool enabled = false)
            : ShortLivedEntity({0,0}, 0),hooks(hooks), enabled(enabled), closest_airlock(nullptr){
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
            if (!enabled)
                return;

            if (closest_airlock == nullptr)
                return;

            renderAirlock(renderer,context);
        }

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        void enable();

        void disable();

        QueueOrder::Value getQueueOrder() override {return QueueOrder::LAST;}
    };
}
