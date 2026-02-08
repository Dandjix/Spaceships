//
// Created by timon on 2/7/26.
//

#include "Door.h"

#include "DoorPanel.h"
#include "game/Update.h"
#include "physics/shapes/RectStaticPhysicsShape.h"

RectStaticPhysicsShape * Door::getDoorLeftShape() {
    return dynamic_cast<RectStaticPhysicsShape *>(door_left->shape);
}

RectStaticPhysicsShape * Door::getDoorRightShape() {
    return dynamic_cast<RectStaticPhysicsShape *>(door_right->shape);
}

Vector2Int Door::getDoorPosition(bool right) const {
    Vector2Int closed_position = {(dimensions.x / 2 )/ 2,};
    Vector2Int open_position = {dimensions.x / 2, 0};
    Vector2Int local_position = Vector2Int::lerp(open_position, closed_position, state);

    if (!right)
        local_position *= -1;

    local_position = local_position.rotate(getAngle());
    return getPosition() + local_position;
}


float Door::getDoorAngle(bool right) const {
    float angle = getAngle();
    if (!right)
        angle = fmod(angle + 180, 360);
    return angle;
}

Vector2Int Door::getDoorDimensions(bool right) const {
    return {static_cast<int>((dimensions.x / 2)), dimensions.y};
}

SDL_Texture * Door::floor_texture = nullptr;


Door::Door(Vector2Int position, float angle, float state, float moment, EntityId::entityId entity_id,
           Vector2Int dimensions)
    : Entity(position, angle), entity_id(entity_id),
      dimensions(dimensions),
      state(state), moment(moment),
      door_left(new DoorPanel(getDoorPosition(false), getDoorAngle(false), dimensions)),
      door_right(new DoorPanel(getDoorPosition(true), getDoorAngle(true), dimensions)) {
    on_destroyed.subscribe([]() {

    });
}

Door::~Door() {
    // delete door_left; //since those two are registered entities, they already get deleted when the spaceship is deleted
    // delete door_right;
}

void Door::update(const UpdateContext &context) {
    state += moment * context.deltaTime;
    state = std::clamp(state,0.0f,1.0f);

    door_left->setPosition(getDoorPosition(false));
    door_left->setAngle(getDoorAngle(false));
    getDoorLeftShape()->setDimensions(getDoorDimensions(false));

    door_right->setPosition(getDoorPosition(true));
    door_right->setAngle(getDoorAngle(true));
    getDoorRightShape()->setDimensions(getDoorDimensions(true));
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
    auto json = Entity::toJson();
    json["state"] = state;
    json["moment"] = moment;
    json["dimensions"] = dimensions.toJson();
    json["entity_id"] = entity_id;
    return json;
}

Entity *Door::initializeRendering(const EntityRendering::Context &context) {
    auto set = context.usage_map.subscribe("objects/door");
    floor_texture = set->at("floor");
    door_left->initializeRendering(context);
    door_right->initializeRendering(context);
    return this;
}

Entity *Door::finalizeRendering(const EntityRendering::Context &context) {
    context.usage_map.unsubscribe("objects/door");
    return this;
}

void Door::render(SDL_Renderer *renderer, const RenderingContext &context) {
}

void Door::registerInSpaceship(SpaceShip *space_ship) {
    Entity::registerInSpaceship(space_ship);
    door_left->registerInSpaceship(space_ship);
    door_right->registerInSpaceship(space_ship);
}

void Door::unregisterInSpaceship(SpaceShip *space_ship, bool delete_when_done) {
    door_left->unregisterInSpaceship(space_ship,delete_when_done);
    door_right->unregisterInSpaceship(space_ship,delete_when_done);
    Entity::unregisterInSpaceship(space_ship, delete_when_done);
}
