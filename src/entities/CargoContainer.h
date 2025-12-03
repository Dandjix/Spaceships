#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../math/Vectors.h"
#include "../game/Rendering.h"
#include "physics/PhysicsEntity.h"
#include "../physics/shapes/RectPhysicsShape.h"

class CargoContainer : public PhysicsEntity {
public:
    enum class Variation {
        blank,
        EMA,
        SL,
        SN,
        COUNT // count is at the end and allows to know the number of enum values
    };

private:
    static SDL_Texture* textures[static_cast<int>(Variation::COUNT)];
    static bool texturesLoaded;

    Variation variation;
    SDL_Color color;
    Vector2Float scale;

public:
    // Constructor
    explicit CargoContainer(Vector2Int position, float angle = 0.0f, Variation variation = Variation::blank, const Vector2Float scale = Vector2Float(61,24).scaleToWorldPosition())
    :
    PhysicsEntity(position, angle,new RectPhysicsShape(this, scale)), variation(variation), scale(scale), color(getRandomColor())
    {}

    // Static method to load textures
    static void LoadTextures(SDL_Renderer* renderer);

    // Override update method (may not be necessary for CargoContainer)
    void update(const UpdateContext & context) override;

    // Override render method
    void render(SDL_Renderer* renderer, const RenderingContext& context) override;

    static Entity * fromJson(nlohmann::json::const_reference json) {
        return new CargoContainer(Vector2Int::fromJson(json["position"]),json["angle"],json["variation"],Vector2Float::fromJson(json["scale"]));
    }
    nlohmann::json toJson() override {
        auto json = Entity::toJson();
        json["variation"] = variation;
        json["scale"] = scale.toJson();
        return json;
    }

private:
    // Helper method to generate random color
    SDL_Color getRandomColor();

public:
    constexpr std::string getJsonType() override{return "cargo_container";}
};

