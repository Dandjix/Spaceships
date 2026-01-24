//
// Created by timon on 11/28/25.
//

#pragma once
#include <utility>

#include "../../entities/scripts/Entity.h"
#include "../../entities/scripts/ShortLivedEntity.h"
#include "EntityRegistry/EntityPlacement.h"


namespace EntityPlacement {
    enum Precision {
        Center,
        Half,
        Quarter,
        Tenth,
        Free
    };

    class EntityPlacer : public ShortLivedEntity {
    private:
        const Registry * registry;

        std::vector<Entity*> * placed_entities;

        std::string to_place;

        float angle_to_place_at = 0;

        Precision precision;

        void placeEntity(Vector2Int world_position, float angle, const std::string &to_place_key);

    public:
        explicit EntityPlacer(
            const Registry * registry,
            std::vector<Entity*> * placed_entities,
            Precision precision,
            bool enabled
        ) : ShortLivedEntity({0, 0}, 0),
            registry(registry),placed_entities(placed_entities), enabled(enabled),precision(precision) {
            to_place = registry->spawners.begin()->first;
        }

        bool enabled;

        void setEntityToPlace(const std::string &option) {
            to_place = option;
        }

        void enable() {
            enabled = true;
        }

        void disable() {
            enabled = false;
        }

        void setPrecision(Precision value);

        void update(const UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

        void render(SDL_Renderer *renderer, const RenderingContext &context) override;

        QueueOrder::Value getQueueOrder() override { return QueueOrder::MIDDLE; }
    };
}
