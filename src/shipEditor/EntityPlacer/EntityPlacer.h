//
// Created by timon on 11/28/25.
//

#pragma once
#include <utility>

#include "entities/Entity.h"
#include "entities/ShortLivedEntity.h"
#include "EntityRegistry/EntityPlacement.h"


namespace EntityPlacement {

    enum Precision {
        Center,
        Half,
        Quarter,
        Tenth,
        Free
    };

class EntityPlacer : ShortLivedEntity {
private:
    Registry registry;

    std::string to_place;

    float angle_to_place_at = 0;

    void placeEntity(Vector2Int world_position, float angle, std::string to_place_key);

    public:
        explicit EntityPlacer(Registry registry, bool enabled) : ShortLivedEntity({0,0},0), registry(std::move(registry)), enabled(enabled) {
            to_place = registry.spawners.begin()->first;
        }

        bool enabled;

        void enable() {
            enabled = true;
        }

        void disable() {
            enabled = false;
        }

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        unsigned short getQueueOrder() override {return 1000;}
    };

}
