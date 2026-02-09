#pragma once
#include "behavior/BehavioredEntity.h"
#include "shipEditor/EntityPlacer/EntityPlacement/EntityPlacement.h"
#include "spaceships/EntityData/EntityLoading.h"


class Vehicle;

class Humanoid : public BehavioredEntity {
private:
    SDL_Texture *texture;

public:
    Event<Vehicle *> on_start_piloting_vehicle;
    Event<Vehicle *> on_stop_piloting_vehicle;

protected:
    float radius;

    Behavior *behavior;

public:
    Humanoid(Vector2Int position, std::optional<float> angle, Behavior *behavior);

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    nlohmann::json toJson() override {
        auto json = Entity::toJson();

        if (behavior != nullptr) {
            json["behavior"] = behavior->toJson();

            if (behavior->isPlayerBehavior()) {
                json["is_player"] = true;
            }
        }
        return json;
    }

    FROM_JSON_DECLARATION(Humanoid,"humanoid");

    EDITOR_PLACE_DECLARATION(Humanoid,"humanoid");

    void setBehavior(Behavior *value) override { behavior = value; }

    Behavior *getBehavior() const override { return behavior; }

    Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

    Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }
};
