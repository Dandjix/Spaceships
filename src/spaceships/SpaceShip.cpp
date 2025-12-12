#include <queue>
#include <unordered_set>

#include "SpaceShip.h"
#include "TileRendering.h"
#include "../math/Hash.h"
#include "ConnectRoomGraph.h"
#include "entities/scripts/ActiveWhenPausedEntity.h"
#include "EntityData/EntityLoading.h"
#include "physics/PhysicsEntity.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorWall.h"
#include "spaceshipTiles/SpaceshipTiles.h"
#include "entities/scripts/LateUpdateEntity.h"
#include "exterior/exteriors/TestExterior.h"

bool EntityComparison::compareEntities(Entity *e1, Entity *e2) {
    return e1->getQueueOrder() < e2->getQueueOrder();
}

bool EntityComparison::comparePhysicsEntities(PhysicsEntity *e1, PhysicsEntity *e2) {
    //TODO : add a queue order for the physics entities if necessary
    return e1->getQueueOrder() < e2->getQueueOrder();
}

void SpaceShip::populateRooms() {
    int width = spaceship_tiles.size_x();
    int height = spaceship_tiles.size_y();

    std::vector visited(width, std::vector<bool>(height, false));

    for (int x = 0; x < spaceship_tiles.size_x(); ++x) {
        for (int y = 0; y < spaceship_tiles.size_y(); ++y) {
            if (shouldSkipTile(x, y, visited))
                continue;
            std::unordered_set<Vector2Int> connected = collectConnectedFloorTiles(x, y, visited);
            Room *newRoom = createRoomFromTiles(connected);
            rooms.addVertex(newRoom);
        }
    }

    ConnectRoomGraph::Connect(rooms);
}

bool SpaceShip::roomsAreDone() const {
    for (int x = 0; x < spaceship_tiles.size_x(); x++) {
        for (int y = 0; y < spaceship_tiles.size_y(); ++y) {
            Tile tile = spaceship_tiles.get_tile(x, y);

            if (tile == Tile::Void || tile == Tile::Wall)
                continue;
            for (Room *room: rooms.getVertices()) {
                if (!room->IncludesTilePosition(x, y))
                    return false;
            }
        }
    }
    return true;
}


bool SpaceShip::shouldSkipTile(int x, int y, const std::vector<std::vector<bool> > &visited) const {
    if (spaceship_tiles.get_tile(x, y) != Tile::Floor)
        return true;

    if (visited[x][y])
        return true;

    for (Room *room: rooms.getVertices()) {
        if (room->IncludesTilePosition(x, y))
            return true;
    }

    return false;
}

std::unordered_set<Vector2Int> SpaceShip::collectConnectedFloorTiles(int startX, int startY,
                                                                     std::vector<std::vector<bool> > &visited) const {
    std::queue<Vector2Int> q;
    std::unordered_set<Vector2Int> connectedTiles;

    q.push(Vector2Int(startX, startY));
    visited[startX][startY] = true;

    std::vector<Vector2Int> directions = {
        Vector2Int(1, 0), Vector2Int(-1, 0),
        Vector2Int(0, 1), Vector2Int(0, -1)
    };

    while (!q.empty()) {
        Vector2Int current = q.front();
        q.pop();
        connectedTiles.insert(current);

        for (const Vector2Int &dir: directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (nx >= 0 && ny >= 0 && nx < spaceship_tiles.size_x() && ny < spaceship_tiles.size_y()) {
                if (!visited[nx][ny] && spaceship_tiles.get_tile(nx, ny) == Tile::Floor) {
                    q.emplace(nx, ny);
                    visited[nx][ny] = true;
                }
            }
        }
    }

    return connectedTiles;
}

Room *SpaceShip::createRoomFromTiles(const std::unordered_set<Vector2Int> &tiles) const {
    return new Room(tiles); // Assumes you have this constructor or similar method
}

Vector2Int SpaceShip::nextNonRoomCoords(int startX, int startY) {
    for (int x = startX; x < spaceship_tiles.size_x(); x++) {
        for (int y = startY; y < spaceship_tiles.size_y(); y++) {
            Tile tile = spaceship_tiles.get_tile(x, y);
            if (tile == Tile::Void || tile == Tile::Wall)
                continue;
            for (Room *room: rooms.getVertices()) {
                if (!room->IncludesTilePosition(x, y))
                    return Vector2Int(x, y);
            }
        }
    }
    return Vector2Int(-1, -1);
}

void SpaceShip::renderRooms(SDL_Renderer *renderer, const RenderingContext &context,
                            const std::vector<Room *> &to_render) const {
    for (const Room *room: to_render) {
        Vector2Int TL, BR;
        room->Encompassing(TL, BR);


        for (int x = TL.x; x <= BR.x; x++) {
            for (int y = TL.y; y <= BR.y; y++) {
                if (!room->IncludesTilePosition(x, y)) {
                    continue;
                }

                Tile tile = spaceship_tiles.get_tile(x, y);

                if (tile == Tile::Void) {
                    continue;
                }
                //SDL_Log("tile : %s",Tiles::nameFromTile(tile).c_str());
                TileRendering::renderTile(renderer, context, tile, x, y);
            }
        }
    }
}

Vector2Int SpaceShip::getCenterOffset() const {
    Vector2Int center_offset = {
        (
            (spaceship_tiles.minimum_x() + spaceship_tiles.maximum_x()) / 2) * Tiles::tileSizePx,
        (spaceship_tiles.minimum_y() + spaceship_tiles.maximum_y() / 2) * Tiles::tileSizePx
    };
    return center_offset.scaleToWorldPosition();
}

SpaceShip::SpaceShip(const SpaceShipBlueprint *blueprint, const std::vector<Entity *> &entities, Vector2Int position,
                     float angle)
    : spaceship_tiles(SpaceshipTiles(blueprint->tiles)), hooks(blueprint->hooks), blueprint_path(blueprint->path),
      position(position), angle(angle),
      exterior(new SpaceShipResources::TestExterior(Vector2Int(1024, 1024).scaleToWorldPosition())) {
    populateRooms();
    registerEntities(entities);
}

SpaceShip::SpaceShip() : spaceship_tiles(SpaceshipTiles({})), hooks({}, {}), blueprint_path(""), angle(0),
                         exterior(new SpaceShipResources::TestExterior(Vector2Int(1024, 1024).scaleToWorldPosition())) {
    populateRooms();
}

SpaceShip::~SpaceShip() {
    for (auto entity: entities) {
        delete entity;
    }
    delete exterior;
}

const SpaceshipTiles &SpaceShip::getSpaceshipTiles() const {
    return spaceship_tiles;
}

void SpaceShip::renderExterior(SDL_Renderer *renderer, const ExteriorRenderingContext &context) {
    exterior->render(renderer,context,this);
}

void SpaceShip::renderInterior(SDL_Renderer *renderer, const RenderingContext &context) {
    if (focusRoom) {
        auto visible = rooms.connected(focusRoom, 2);
        std::vector<Room *> visibleRoomsVector(visible.begin(), visible.end());
        visibleRoomsVector.push_back(focusRoom);
        //std::vector<Room*> visibleRoomsVector = this->rooms.getVertices();
        //SDL_Log("rendering %d rooms (%d)",visibleRoomsVector.size(), visible.size());
        renderRooms(renderer, context, visibleRoomsVector);
    }

    for (Room *room: rooms.getVertices()) {
        if (!room->getBoundingBoxes().empty()) {
            std::pair<Vector2Int, Vector2Int> bb = room->getBoundingBoxes()[0];
            int r, g, b;
            Hash::getRandomColor(bb.first.x, bb.first.y, &r, &g, &b);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        }
        std::vector<std::pair<Vector2Int, Vector2Int> > boxes = room->getBoundingBoxes();
        for (const std::pair<Vector2Int, Vector2Int> &bb: boxes) {
            DebugRendering::drawWorldRoomBoundingBox(renderer, context, bb.first, bb.second);
        }
    }
}

std::vector<Entity *> SpaceShip::getEntities(RoomDistance queue, Room *reference_room) const {
    auto vec = std::vector<Entity *>(entities.begin(), entities.end());

    std::sort(vec.begin(), vec.end(), EntityComparison::compareEntities);

    return vec;
}

std::vector<PhysicsEntity *> SpaceShip::getPhysicsEntities(RoomDistance queue, Room *reference_room) const {
    auto vec = std::vector<PhysicsEntity *>(physics_entities.begin(), physics_entities.end());

    std::sort(vec.begin(), vec.end(), EntityComparison::comparePhysicsEntities);

    return vec;
}

std::vector<BehavioredEntity *> SpaceShip::getBehavioredEntities(RoomDistance queue, Room *reference_room) const {
    //TODO if in need of performance, store in its own variable
    std::vector<BehavioredEntity *> to_return = {};
    for (auto entity: entities) {
        if (auto behaviored = dynamic_cast<BehavioredEntity *>(entity)) {
            to_return.push_back(behaviored);
        }
    }
    return to_return;
}

void SpaceShip::Dock(SpaceShip other) {
}

void SpaceShip::registerEntities(const std::vector<Entity *> &to_register) {
    for (Entity *e: to_register) {
        e->registerInSpaceship(this);
    }
}

void SpaceShip::handleQueueDeletion() {
    for (auto e: deletion_queue) {
        delete e;
    }
}

void SpaceShip::unregisterEntities(const std::vector<Entity *> &to_unregister) {
    for (Entity *e: to_unregister) {
        e->unregisterInSpacehip(this);
    }
}

bool SpaceShip::has_entity(Entity *entity) const {
    return entities.contains(entity);
}

void SpaceShip::update(const UpdateContext &context) {
    if (!focusEntity) {
        focusRoom = nullptr;
        return;
    }

    for (Room *room: rooms.getVertices()) {
        if (room->IncludesWorldPosition(focusEntity->getPosition())) {
            //SDL_Log("There is a room !");
            focusRoom = room;
            return;
        }
    }
    //SDL_Log("There is no room.");
    focusRoom = nullptr;
}

void SpaceShip::setPlayer(Entity *value) {
    focusEntity = value;
}

SpaceShip *SpaceShip::fromJson(nlohmann::json::const_reference json) {
    SpaceShipBlueprint *blueprint = SpaceShipBlueprint::load(json["blueprint_path"]);

    std::vector<Entity *> loaded_entities = {};
    for (const auto &entity_entry: json["entities"]) {
        loaded_entities.push_back(EntityLoading::fromJson(entity_entry));
    }
    auto *space_ship = new SpaceShip(blueprint, loaded_entities, Vector2Int::fromJson(json["position"]), json["angle"]);

    delete blueprint;

    return space_ship;
}

nlohmann::json SpaceShip::toJson() {
    nlohmann::json json = {};

    json["blueprint_path"] = blueprint_path;
    json["position"] = position.toJson();
    json["angle"] = angle;

    std::vector<nlohmann::json> entity_entries = nlohmann::json::array();
    for (auto e: entities) {
        if (e->isJsonSerializable()) {
            entity_entries.push_back(e->toJson());
        }
    }

    json["entities"] = entity_entries;

    return json;
}


// Handling ------------------------------------------------------------------------------------------------------------

void SpaceShip::physicsHandling(float target_delta_time, int subdivisions) {
    const PhysicsUpdateContext physicsContext = {
        target_delta_time / static_cast<float>(subdivisions),
        this
    };

    for (int i = 0; i < subdivisions; ++i) {
        auto working_physics_entities = getPhysicsEntities(RoomDistance::Close);

        for (int j = 0; j < working_physics_entities.size(); j++) {
            for (int k = j + 1; k < working_physics_entities.size(); k++) {
                const auto e1 = working_physics_entities.at(j);
                auto e2 = working_physics_entities.at(k);

                if (!e1->shape->getBoundingBox().intersects(e2->shape->getBoundingBox())) {
                    continue;
                }

                PhysicsUpdateVisitor *visitor = e1->shape->createVisitor();

                e2->shape->consumeVisitor(visitor, this);

                delete visitor;
            }
        }
    }

    for (PhysicsEntity *e: getPhysicsEntities(RoomDistance::All)) {
        PhysicsUpdateVisitorWall visitor = PhysicsUpdateVisitorWall();
        e->shape->consumeVisitor(&visitor, this);
    }


    for (PhysicsEntity *entity: getPhysicsEntities(RoomDistance::All)) {
        entity->shape->physicsUpdate(physicsContext);
    }
}

void SpaceShip::renderEntities(SDL_Renderer *renderer, RenderingContext renderingContext) {
    for (Entity *entity: getEntities(RoomDistance::Immediate)) {
        entity->render(renderer, renderingContext);
    }
    //render debug
    for (Entity *entity: getEntities(RoomDistance::Immediate)) {
        entity->debugRender(renderer, renderingContext);
    }
    //physics shapes debug
    for (PhysicsEntity *entity: getPhysicsEntities(RoomDistance::Immediate)) {
        entity->shape->debugRender(renderer, renderingContext);
    }
}

void SpaceShip::eventHandling(const SDL_Event &event, const GameEvent::GameEventContext &event_context, bool paused) {
    if (!paused) {
        for (Entity *entity: getEntities(RoomDistance::All)) {
            entity->handleEvent(event, event_context);
        }
    } else {
        for (auto entity: active_when_paused_entities) {
            entity->handleEvent(event, event_context);
        }
    }
}

void SpaceShip::updateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                               GameEvent::MousePositionType mouse_position_type, bool paused) {
    UpdateContext updateContext = {
        camera_info,
        deltaTime,
        this,
        mouse_position_type
    };

    if (!paused) {
        // update
        for (Entity *entity: getEntities(RoomDistance::All)) {
            entity->update(updateContext);
        }
        hooks.update(updateContext);
    } else {
        for (auto entity: active_when_paused_entities) {
            entity->update(updateContext);
        }
    }


    update(updateContext);
}

void SpaceShip::lateUpdateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                                   GameEvent::MousePositionType mouse_position_type, bool paused) {
    if (paused)
        return;

    UpdateContext updateContext = {
        camera_info,
        deltaTime,
        this,
        mouse_position_type
    };

    // late update
    for (LateUpdateEntity *entity: late_update_entities) {
        entity->lateUpdate(updateContext);
    }
}
