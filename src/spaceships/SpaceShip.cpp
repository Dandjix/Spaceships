#include <queue>
#include <unordered_set>

#include "SpaceShip.h"
#include "TileRendering.h"
#include "../math/Hash.h"
#include "ConnectRoomGraph.h"
#include "SpaceShipBlueprint.h"
#include "behavior/BehavioredEntity.h"
#include "entities/entityId/EntityId.h"
#include "entities/scripts/ActiveWhenPausedEntity.h"
#include "EntityData/EntityLoading.h"
#include "../physics/scripts/PhysicsEntity.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorWall.h"
#include "spaceshipTiles/SpaceshipTiles.h"
#include "entities/scripts/LateUpdateEntity.h"
#include "exterior/exteriors/TestExterior.h"
#include "physics/Physics.h"
#include "physics/shapes/PhysicsShape.h"

namespace GameState {
    struct transientGameState;
}


Vector2Int SpaceShip::getCenterOffset() const {
    Vector2Int center_offset = {
        (
            (spaceship_tiles->minimum_x() + spaceship_tiles->maximum_x()) / 2) * Tiles::tileSizePx,
        (spaceship_tiles->minimum_y() + spaceship_tiles->maximum_y() / 2) * Tiles::tileSizePx
    };
    return center_offset.scaleToWorldPosition();
}

Vector2Int SpaceShip::getCenter() const {
    return position + getCenterOffset().rotate(getAngle());
}


SpaceShip::SpaceShip(const SpaceShipBlueprint *blueprint,
                     const std::vector<Entity *> &instance_entities,
                     Vector2Int position,
                     float angle)
    : exterior(new SpaceShipResources::TestExterior(Vector2Int(1024, 1024).scaleToWorldPosition())),
      blueprint_path(blueprint->path),
      spaceship_tiles(new SpaceshipTiles(blueprint->tiles)),
      spaceship_hooks(blueprint->hooks),
      position(position),
      angle(angle) {

    std::vector<Entity *> entities_to_add;
    entities_to_add.reserve(blueprint->entities.size() + instance_entities.size());
    for (Entity * e: blueprint->entities)entities_to_add.push_back(e);
    for (Entity * e: instance_entities)entities_to_add.push_back(e);


    instance = new Instances::Instance(this, entities_to_add);

    spaceship_rooms = new SpaceshipRooms(spaceship_tiles);
    spaceship_rooms->populateRooms();
}

SpaceShip::SpaceShip()
    : exterior(nullptr),
      blueprint_path(""),
      spaceship_tiles(new SpaceshipTiles({})),
      spaceship_hooks(new SpaceshipHooks({}, {}, {})),
      instance(new Instances::Instance(this, {})),
      position({0, 0}),
      angle(0) {
    spaceship_rooms = new SpaceshipRooms(spaceship_tiles);
    spaceship_rooms->populateRooms();
}


SpaceShip::~SpaceShip() {
    delete spaceship_tiles;
    delete spaceship_hooks;
    // delete spaceship_rooms; //TODO : find why I cannot do that (double free nonsense)
    delete instance;
    delete exterior;
}

void SpaceShip::renderExterior(SDL_Renderer *renderer, const ExteriorRenderingContext &context) const {
    exterior->render(renderer, context, this);
}


void SpaceShip::Dock(SpaceShip other) {
}

SpaceShip *SpaceShip::fromJson(nlohmann::json::const_reference json,
                               GameState::transientGameState &transient_game_state,
                               EntityId::Manager &entity_id_manager,
                               EntityRendering::Context *entity_rendering_context) {
    SpaceShipBlueprint *blueprint = SpaceShipBlueprint::load(json["blueprint_path"], transient_game_state,
                                                             entity_id_manager, entity_rendering_context, false);


    std::vector<Entity *> loaded_entities = Instances::Instance::entitiesFromJson(
        json["instance"], transient_game_state, entity_rendering_context);

    auto *space_ship = new SpaceShip(blueprint, loaded_entities,
                                     Vector2Int::fromJson(json["position"]), json["angle"]);

    delete blueprint;

    return space_ship;
}

SpaceShip *SpaceShip::finalizeRendering(const EntityRendering::Context &entity_rendering_context) {
    instance->finalizeRendering(entity_rendering_context);
    return this;
}

nlohmann::json SpaceShip::toJson() {
    nlohmann::json json = {};

    json["blueprint_path"] = blueprint_path;
    json["position"] = position.toJson();
    json["angle"] = angle;

    json["instance"] = instance->entitiesToJson();

    return json;
}
