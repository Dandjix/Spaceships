#pragma once
#include "../game/Rendering.h"
#include "../math/AdjacencyListGraph.h"
#include "Room.h"
#include "SpaceShipBlueprint.h"
#include <initializer_list>
#include <queue>
#include <unordered_set>

#include "spaceshipTiles/SpaceshipTiles.h"

class PhysicsEntity;
class Entity;

enum class RoomDistance {
  All,
  Immediate,
  Close,
  Far,
};

namespace EntityComparison {
inline static bool compareEntities(Entity *e1, Entity *e2);
inline static bool comparePhysicsEntities(PhysicsEntity *e1, PhysicsEntity *e2);
}

class SpaceShip {
private:
  void populateRooms();
  bool roomsAreDone();

  bool shouldSkipTile(int x, int y,
                      const std::vector<std::vector<bool>> &visited) const;
  std::unordered_set<Vector2Int>
  collectConnectedFloorTiles(int startX, int startY,
                             std::vector<std::vector<bool>> &visited) const;

  Room *createRoomFromTiles(const std::unordered_set<Vector2Int> &tiles) const;

  Vector2Int nextNonRoomCoords(int startX = 0, int startY = 0);

  /// <summary>
  /// the room which has focus. Generally the room the player is in.
  /// </summary>
  Room *focusRoom = nullptr;
  Entity *focusEntity = nullptr;

  void renderRooms(SDL_Renderer *renderer, const RenderingContext &context,
                   const std::vector<Room *> &rooms);

protected:
  AdjacencyListGraph<Room *> rooms;
  SpaceshipTiles spaceship_tiles;
  std::vector<Entity*> deletion_queue;

public:
  std::unordered_set<Entity *> entities;
  std::unordered_set<PhysicsEntity *> physics_entities;

  SpaceshipHooks hooks;

  explicit SpaceShip(SpaceShipBlueprint *blueprint);
  explicit SpaceShip();

  const SpaceshipTiles &getSpaceshipTiles() const;

  /// <summary>
  /// renders the hull of the ship
  /// </summary>
  void renderExterior(SDL_Renderer *renderer, const RenderingContext &context);

  /// <summary>
  /// renders the visible rooms of the ship
  /// </summary>
  void renderInterior(SDL_Renderer *renderer, const RenderingContext &context);

  /// <summary>
  /// returns the entities (read only)
  /// @param reference_room the room to which this is Immediate, close etc. By default (nullptr) this will be relative to the player
  /// </summary>
  /// <returns></returns>
  std::vector<Entity *> getEntities(RoomDistance queue, Room* reference_room = nullptr) const;

  /// <summary>
  /// returns the physics entities (read only)
  /// </summary>
  /// <returns></returns>
  std::vector<PhysicsEntity *> getPhysicsEntities(RoomDistance queue, Room* reference_room = nullptr) const;
  /// <summary>
  /// docks an other ship to this ship. You will want to align them relatively
  /// well since this will snap the other ship. Undocking is future me's problem
  /// </summary>
  /// <param name="other"> The other ship</param>
  void Dock(SpaceShip other);

  /// <summary>
  /// adds one or more entries to the entities map
  /// </summary>
  void registerEntities(std::initializer_list<Entity *> entities);

  void queueDelete(Entity * entity) {
    deletion_queue.push_back(entity);
  }

  void handleQueueDeletion();

  void physicsHandling(float target_delta_time, int subdivisions = 2);

  void renderEntities(SDL_Renderer *renderer, RenderingContext renderingContext);

  void eventHandling(const SDL_Event &event, const GameEvent::GameEventContext &event_context);

  void updateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime, GameEvent::MousePositionType mouse_position_type);

  /// <summary>
  /// removes one or more entries from the entities map. Used for when the
  /// object is destroyed or if another ship undocks prolly idk
  /// </summary>
  /// <param name="entities"> the entiti(es) to unregister</param>
  /// <returns></returns>
  void unregisterEntities(std::initializer_list<Entity *> entities);

  void update(const UpdateContext &context);

  void setFocusEntity(Entity *entity);
};