//
// Created by timon on 2/7/26.
//

#include "Door.h"

#include "physics/shapes/RectStaticPhysicsShape.h"

Door::Door(Vector2Int position, float angle, float state, float moment, Vector2Int dimensions)
    : PhysicsEntity(position, angle, new RectStaticPhysicsShape(this, dimensions)), dimensions(dimensions),
      state(state), moment(moment) {
}

FROM_JSON_DEFINITION(Door) {
    return new Door(
        Vector2Int::fromJson(json["position"]),
        json["angle"],
        json["state"],
        json["moment"],
        Vector2Int::fromJson(json["dimensions"])
    );
}

nlohmann::json Door::toJson() {
    auto json = PhysicsEntity::toJson();
    json["state"] = state;
    json["moment"] = moment;
    json["dimensions"] = dimensions.toJson();
    return json;
}

Entity *Door::initializeRendering(const EntityRendering::Context &context) {
    // door_texture = context.usage_map.subscribe("objects/door")->at("door");
    // floor_texture = context.usage_map.subscribe("objects/door")->at("floor");
    return this;
}

Entity *Door::finalizeRendering(const EntityRendering::Context &context) {
    return this;
}

void Door::render(SDL_Renderer *renderer, const RenderingContext &context) {
}
