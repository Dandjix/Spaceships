#include "CargoContainer.h"

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "physics/shapes/RectPhysicsShape.h"
#include "textures/TextureSet.h"
#include "textures/UsageMap.h"

CargoContainer::CargoContainer(Vector2Int position, float angle, Variation variation, Vector2Float scale,
                               Color color) : PhysicsEntity(position, angle, new RectPhysicsShape(this, scale)),
                                              variation(variation), scale(scale), color(color) {
    auto texture_set = Textures::UsageMap::getInstance().subscribe("objects/cargoContainer");
    switch (variation) {
        case Variation::blank:
            texture = texture_set->at("blank");
            break;
        case Variation::EMA:
            texture = texture_set->at("EMA");
            break;
        case Variation::SL:
            texture = texture_set->at("SL");
            break;
        case Variation::SN:
            texture = texture_set->at("SN");
            break;
    }
}

CargoContainer::~CargoContainer() {
    Textures::UsageMap::getInstance().unsubscribe("objects/cargoContainer");
}

nlohmann::json CargoContainer::toJson() {
    auto json = Entity::toJson();
    json["variation"] = variation;
    json["scale"] = scale.toJson();
    json["color"] = color.toJson();
    return json;
}

constexpr std::string CargoContainer::getJsonType() { return "cargo_container"; }

void CargoContainer::update(const UpdateContext &context) {
    // CargoContainer might not need to move, so leave it empty.
}

void CargoContainer::render(SDL_Renderer *renderer, const RenderingContext &context) {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

    // Calculate the half-size of the container
    Vector2Float halfSize = scale.scaleToScreenPosition() * 0.5f;

    // Render the texture
    renderTexture(renderer, context, texture, halfSize);
}

Entity *CargoContainer::fromJson(nlohmann::json::const_reference json,
                                 GameState::transientGameState &transient_game_state) {
    return new CargoContainer(Vector2Int::fromJson(
    json["position"]),
    json["angle"],
    json["variation"],
    Vector2Float::fromJson(json["scale"]),
    Color::fromJson(json["color"]));
}

Color CargoContainer::getRandomColor() {
    unsigned char r = 128 + rand() % 127;
    unsigned char g = 128 + rand() % 127;
    unsigned char b = 128 + rand() % 127;

    return {r, g, b, 255};
}
