//
// Created by timon on 2/7/26.
//

#include "Door.h"

#include "DoorPanel.h"
#include "game/Update.h"
#include "physics/ConvexCast.h"
#include "physics/shapes/RectStaticPhysicsShape.h"
#include "rendering/util/RenderTexture.h"
#include "shipEditor/EntityPlacer/EntityPlacement/EntityPlacement.h"

RectStaticPhysicsShape *Door::getDoorLeftShape() {
    return dynamic_cast<RectStaticPhysicsShape *>(door_left->shape);
}

RectStaticPhysicsShape *Door::getDoorRightShape() {
    return dynamic_cast<RectStaticPhysicsShape *>(door_right->shape);
}

[[nodiscard]] int getDetectorWidth() {
    return Scaling::metricToWorld(0.15f);
}

Vector2Int Door::getDoorPosition(bool right) const {
    Vector2Int closed_position = {dimensions.x / 4, 0};
    Vector2Int open_position = {(dimensions.x / 4 + dimensions.x / 2), 0};
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

Vector2Int computeDoorDimensions(bool right, Vector2Int dimensions) {
    return {(dimensions.x / 2), dimensions.y};
}

Vector2Int Door::getDoorDimensions(bool right) const { return computeDoorDimensions(right, dimensions); }

SDL_Texture *Door::floor_texture = nullptr;


Door::Door(Vector2Int position, float angle, float state, float moment, EntityId::entityId entity_id,
           Vector2Int dimensions)
    : Entity(position, angle), entity_id(entity_id),
      dimensions(dimensions),
      state(state), moment(moment),
      door_left(new DoorPanel(getDoorPosition(false), getDoorAngle(false), computeDoorDimensions(false, dimensions))),
      door_right(new DoorPanel(getDoorPosition(true), getDoorAngle(true), computeDoorDimensions(true, dimensions))) {
    on_destroyed.subscribe([]() {
    });
}

Door::~Door() {
    // delete door_left; //since those two are registered entities, they already get deleted when the spaceship is deleted
    // delete door_right;
}

Vector2Int Door::getCastPosition(bool right) const {
    Vector2Int closed_position = Vector2Int(0, 0) - Vector2Int(getDetectorWidth() / 2, 0);
    Vector2Int open_position = Vector2Int(dimensions.x / 2, 0) - Vector2Int(getDetectorWidth() / 2, 0);
    Vector2Int local_position = Vector2Int::lerp(open_position, closed_position, state);

    if (!right)
        local_position *= -1;

    local_position = local_position.rotate(getAngle());
    return getPosition() + local_position;
}

Vector2Int Door::getCastDimensions(bool right) const {
    return {getDetectorWidth(), dimensions.y};
}

bool Door::hasOtherEntity(std::vector<PhysicsShape *> cast_res_1) const {
    bool other = false;
    for (auto shape: cast_res_1) {
        if (shape->owner_entity != door_left && shape->owner_entity != door_right)
            other = true;
    }
    return other;
}

void Door::update(const UpdateContext &context) {
    if (moment > 0 && state < 1) {
        std::vector<PhysicsShape *> cast_res_1 = Physics::RectCast(getCastPosition(false), getCastDimensions(false),
                                                                   getAngle(), context.spaceShip);
        std::vector<PhysicsShape *> cast_res_2 = Physics::RectCast(getCastPosition(true), getCastDimensions(true),
                                                                   getAngle(), context.spaceShip);

        if (hasOtherEntity(cast_res_1) || hasOtherEntity(cast_res_2)) {
            moment = -1;
        }
    }


    state += moment * context.deltaTime;
    state = std::clamp(state, 0.0f, 1.0f);


    door_left->setPosition(getDoorPosition(false));
    door_left->setAngle(getDoorAngle(false));
    // getDoorLeftShape()->setDimensions(getDoorDimensions(false));

    door_right->setPosition(getDoorPosition(true));
    door_right->setAngle(getDoorAngle(true));
    // getDoorRightShape()->setDimensions(getDoorDimensions(true));
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
    std::vector<Vector2Int> floor_tiles = {
        Vector2Int(Scaling::scaleToWorld(Tiles::tileSizePx / 2), 0).rotateCardinal(static_cast<int>(getAngle())),
        Vector2Int(Scaling::scaleToWorld(-Tiles::tileSizePx / 2), 0).rotateCardinal(static_cast<int>(getAngle()))
    };

    for (int i = 0; i < floor_tiles.size(); ++i) {
        floor_tiles[i] += getPosition();
    }

    for (auto floor_tile: floor_tiles) {
        Rendering::Util::renderTexture(
            renderer,
            context,
            floor_tile,
            getAngle(),
            floor_texture,
            {Tiles::tileSizePx/2, Tiles::tileSizePx/2});
    }
}

QueueOrder::Value Door::getQueueOrder() { return QueueOrder::FIRST + 1000; }

EntityId::entityId Door::getEntityId() const { return entity_id; }

Entity *Door::asEntity() { return this; }

Toggleable *Door::asToggleable() { return this; }

void Door::toggle() {
    if (moment > 0)
        moment = -1;
    else
        moment = 1;
}

bool Door::getStatus() { return moment > 0; }

void Door::setStatus(bool value) {
    if (value)
        moment = 1;
    else
        moment = -1;
}

void Door::registerInSpaceship(SpaceShip *space_ship) {
    Entity::registerInSpaceship(space_ship);
    door_left->registerInSpaceship(space_ship);
    door_right->registerInSpaceship(space_ship);
}

void Door::unregisterInSpaceship(SpaceShip *space_ship, bool delete_when_done) {
    door_left->unregisterInSpaceship(space_ship, delete_when_done);
    door_right->unregisterInSpaceship(space_ship, delete_when_done);
    Entity::unregisterInSpaceship(space_ship, delete_when_done);
}

int round(int x, int quotient) {
    return static_cast<int>(std::round(static_cast<double>(x) / quotient) * quotient);
}

[[nodiscard]] Vector2Int round_position(Vector2Int position, float angle) {
    int cell_size = Scaling::scaleToWorld(Tiles::tileSizePx);

    if (std::fmod(angle, 180.0f) == 0.0f) {
        //horizontal
        //x rounded to center of cell,
        //y rounded to edge of cell
        position = {
            round(position.x, cell_size) + cell_size / 2,
            round(position.y, cell_size)
        };
    } else {
        //vertical
        //x rounded to edge of cell,
        //y rounded to center of cell
        position = {
            round(position.x, cell_size),
            round(position.y, cell_size) + cell_size / 2
        };
    }
    return position;
}

EDITOR_PLACE_DEFINITION(Door) {
    float angle = context->interface->getAngle();
    Vector2Int position = context->interface->getPosition();

    float rounded_angle = round(angle,90);
    Vector2Int rounded_position = round_position(position, rounded_angle);

    std::cout << "rounded to : " << rounded_angle << "," << rounded_position << std::endl;

    return std::async(std::launch::async, [rounded_position, rounded_angle]()-> Entity * {
        return new Door(rounded_position, rounded_angle, 0, 0, 1234);
    });
}
