#pragma once
#include "physics/PhysicsEntity.h"


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

    static Entity *fromJson(nlohmann::json::const_reference json, GameState::transientGameState &transient_game_state) {
        return new Sphere(Vector2Int::fromJson(json["position"]), json["radius"]);
    }

    nlohmann::json toJson() override {
        auto json = Entity::toJson();
        json["radius"] = radius;
        return json;
    }

    constexpr std::string getJsonType() override { return "sphere"; }
};
