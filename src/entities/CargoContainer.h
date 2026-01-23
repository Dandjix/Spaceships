#pragma once
#include "math/Color.h"
#include "physics/PhysicsEntity.h"
#include "textures/TextureSet.h"

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
    Vector2Float scale;
    SDL_Texture *texture;

public:
    // Constructor
    explicit CargoContainer(
        Vector2Int position,
        float angle = 0.0f,
        Variation variation = Variation::blank,
        Vector2Float scale = Vector2Float(61, 24).scaleToWorldPosition(),
        Color color = getRandomColor());

    ~CargoContainer() override;

    // Override update method (may not be necessary for CargoContainer)
    void update(const UpdateContext &context) override;

    // Override render method
    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    static Entity *fromJson(nlohmann::json::const_reference json, GameState::transientGameState &transient_game_state);

    nlohmann::json toJson() override;

private:
    // Helper method to generate random color
    static Color getRandomColor();

public:
    constexpr std::string getJsonType() override;
};
