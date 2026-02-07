//
// Created by timon on 2/7/26.
//

#include "Door.h"

#include "physics/shapes/RectStaticPhysicsShape.h"

Door::Door(Vector2Int position, float angle, float state, float moment,EntityId::entityId entity_id ,Vector2Int dimensions)
    : PhysicsEntity(position, angle, new RectStaticPhysicsShape(this, dimensions)), entity_id(entity_id),
      dimensions(dimensions),
      state(state), moment(moment) {
}

FROM_JSON_DEFINITION(Door) {
    auto door = new Door(
        Vector2Int::fromJson(json["position"]),
        json["angle"],
        json["state"],
        json["moment"],
        json["entity_id"],
        Vector2Int::fromJson(json["dimensions"])
    );

    transient_game_state.identified_entities[json["entity_id"]] = door;

    return door;
}

nlohmann::json Door::toJson() {
    auto json = PhysicsEntity::toJson();
    json["state"] = state;
    json["moment"] = moment;
    json["dimensions"] = dimensions.toJson();
    json["entity_id"] = entity_id;
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
