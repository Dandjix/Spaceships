#pragma once
#include "math/Color.h"
#include "../physics/scripts/PhysicsEntity.h"
#include "spaceships/EntityData/EntityLoading.h"

class CargoContainer : public PhysicsEntity {
public:
    enum class Variation {
        blank,
        EMA,
        SL,
        SN,
    };

private:
    Variation variation;
    Color color;
    Vector2Int scale;
    SDL_Texture *texture;

public:
    // Constructor
    explicit CargoContainer(
        Vector2Int position,
        float angle = 0.0f,
        Variation variation = Variation::blank,
        Vector2Int scale = Vector2Int(61, 24).scaleToWorldPosition(),
        Color color = getRandomColor());


    void update(const UpdateContext &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    CargoContainer *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;

    FROM_JSON_DECLARATION(CargoContainer,"cargo_container");

    nlohmann::json toJson() override;


private:
    static Color getRandomColor();
};
