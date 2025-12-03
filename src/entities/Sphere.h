#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../math/Vectors.h"
#include "../game/Rendering.h"
#include "physics/PhysicsEntity.h"
#include "../physics/shapes/RoundPhysicsShape.h"

class Sphere : public PhysicsEntity {
private:
    static SDL_Texture* texture;  // Texture for the sphere
    static bool texturesLoaded;   // Flag to check if textures are loaded
    float radius;
public:
    // Constructor
    Sphere(Vector2Int position, float radius)
        : PhysicsEntity(position,  std::nullopt,new RoundPhysicsShape(this,radius)), radius(radius)
    {
    }

    // Static function to load textures
    static void LoadTextures(SDL_Renderer* renderer);

    float get_weight() override { return  5;}

    // Override render function to render the sphere
    void render(SDL_Renderer* renderer, const RenderingContext& context) override;

    // Override update function (currently does nothing)
    void update(const UpdateContext & context) override;

    static Entity * fromJson(nlohmann::json::const_reference json) {
        return new Sphere(Vector2Int::fromJson(json["position"]),json["radius"]);
    }

    nlohmann::json toJson() override {
        auto json = Entity::toJson();
        json["radius"] = radius;
        return json;
    }

    constexpr std::string getJsonType(){return "sphere";}
};
