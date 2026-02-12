#include "CargoContainer.h"

#include "entityRendering/RenderingInitialization.h"
#include "physics/shapes/RectPhysicsShape.h"
#include "shipEditor/EntityPlacer/EntityPlacement/EntityPlacement.h"
#include "textures/TextureSet.h"
#include "textures/UsageMap.h"

CargoContainer::CargoContainer(Vector2Int position,
                               float angle,
                               Variation variation,
                               Color color,
                               Vector2Int scale
)
    : PhysicsEntity(position,
                    angle,
                    new RectPhysicsShape(this, scale)),
      variation(variation), scale(scale), texture(nullptr), color(color) {
}


nlohmann::json CargoContainer::toJson() {
    auto json = Entity::toJson();
    json["variation"] = variation;
    json["color"] = color.toJson();
    return json;
}

void CargoContainer::update(const UpdateContext &context) {
    // CargoContainer might not need to move, so leave it empty.
}

void CargoContainer::render(SDL_Renderer *renderer, const RenderingContext &context) {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

    // Calculate the half-size of the container
    Vector2Float halfSize = Vectors::toVector2Float(scale.scaleToScreenPosition()) * 0.5f;

    // Render the texture
    renderTexture(renderer, context, texture, halfSize);
}

FROM_JSON_DEFINITION(CargoContainer) {
    return new CargoContainer(Vector2Int::fromJson(
                                  json["position"]),
                              json["angle"],
                              json["variation"],
                              Color::fromJson(json["color"]));
}

EDITOR_PLACE_DEFINITION(CargoContainer) {
    Vector2Int position = context->interface->getPosition();
    float angle = context->interface->getAngle();

    return std::async(std::launch::async, [position, angle]()-> Entity * {
        return new CargoContainer(position, angle);
    });
}

Color CargoContainer::getRandomColor() {
    unsigned char r = 128 + rand() % 127;
    unsigned char g = 128 + rand() % 127;
    unsigned char b = 128 + rand() % 127;

    return {r, g, b, 255};
}

CargoContainer *CargoContainer::initializeRendering(const EntityRendering::Context &context) {
    auto texture_set = context.usage_map.subscribe("objects/cargoContainer");

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

    return this;
}

Entity *CargoContainer::finalizeRendering(const EntityRendering::Context &context) {
    context.usage_map.unsubscribe("objects/cargoContainer");
    return this;
}
