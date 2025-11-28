//
// Created by timon on 11/28/25.
//

#pragma once
#include "entities/Entity.h"
#include "game/Rendering.h"
#include "spaceships/Hooks/SpaceshipHooks.h"

namespace HookDeleter{

    class PointDeleter : public Entity{
    private:
        SpaceshipHooks * hooks;
        bool enabled;

        HookPoint * closest_point;

        std::string closest_name;


        void deletePoint(const std::string &point_name);

        void renderPoint(SDL_Renderer *renderer, const RenderingContext &context) const;

    public:
        explicit PointDeleter(SpaceshipHooks * hooks, bool enabled = false)
            : Entity({0,0}, 0),hooks(hooks), enabled(enabled), closest_point(nullptr){
        }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
            if (!enabled)
                return;

            if (closest_point == nullptr)
                return;

            renderPoint(renderer,context);
        }

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        void enable();

        void disable();

        unsigned short getQueueOrder() override {return 100;}
    };

}
