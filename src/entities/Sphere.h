#pragma once
#include "../physics/scripts/PhysicsEntity.h"
#include "spaceships/EntityData/EntityLoading.h"


namespace Textures {
    struct TextureSet;
}

class Sphere : public PhysicsEntity {
private:
    SDL_Texture *texture; // Texture for the sphere
    float radius;

public:
    // Constructor
    Sphere(Vector2Int position, float radius);

    float get_weight() override { return 5; }

    Sphere *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    void update(const UpdateContext &context) override;

    FROM_JSON_DECLARATION(Sphere,"sphere");

    nlohmann::json toJson() override {
        auto json = Entity::toJson();
        json["radius"] = radius;
        return json;
    }
};
