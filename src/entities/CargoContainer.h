#pragma once
#include "math/Color.h"
#include "../physics/scripts/PhysicsEntity.h"
#include "math/Scaling.h"
#include "shipEditor/EntityPlacer/EntityPlacement/EntityPlacement.h"
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
        Color color = getRandomColor(),
        Vector2Int scale = Scaling::metricToWorld(Vector2Float(6.06f, 2.44f))
        );


    void update(const UpdateContext &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    CargoContainer *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;

    FROM_JSON_DECLARATION(CargoContainer,"cargo_container");

    EDITOR_PLACE_DECLARATION(CargoContainer,"cargo_container");

    nlohmann::json toJson() override;


private:
    static Color getRandomColor();

public:
    /**
     * a loaded cargo container is on average about 24 metric tons
     * @return
     */
    float get_weight() override {return 24000;}
};
