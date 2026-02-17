#pragma once
#include "entities/scripts/Entity.h"
#include "gameEvent/MousePositionType.h"
#include "Rooms/SpaceshipRooms.h"
#include "spaceships/entityContainer/EntityContainer.h"
#include "spaceships/EntityData/EntityLoading.h"
#include "spaceships/hashProximityList/HashProximityMap.h"
#include "spaceships/spaceshipTiles/SpaceshipTiles.h"
//
// Created by timon on 2/13/26.
//

struct RenderingContext;
class BehavioredEntity;
class Camera;
class ActiveWhenPausedEntity;
class LateUpdateEntity;
class PhysicsEntity;
class Entity;

namespace CameraTransformations {
    struct CameraInfo;
}

namespace EntityComparison {
    inline static bool compareEntities(Entity *e1, Entity *e2);

    inline static bool comparePhysicsEntities(PhysicsEntity *e1, PhysicsEntity *e2);
}

namespace Instances {
    class Instance {
    public:
        HashProximityMap hash_proximity_map;

        SpaceShip *space_ship;

        EntityContainer<Entity *> entities;
        EntityContainer<PhysicsEntity *> physics_entities;
        EntityContainer<LateUpdateEntity *> late_update_entities;
        EntityContainer<ActiveWhenPausedEntity *> active_when_paused_entities;
        EntityContainer<Camera *> cameras;

        Instance(
            SpaceShip *space_ship,
            const std::vector<Entity *> &entities
        );

        ~Instance() {
            for (Entity *entity: entities) {
                delete entity;
            }
        }

        void physicsHandling(float target_delta_time, int subdivisions = 2);


        void updateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                            GameEvent::MousePositionType mouse_position_type, bool
                            paused);

        void lateUpdateHandling(const CameraTransformations::CameraInfo &camera_info, float deltaTime,
                                GameEvent::MousePositionType mouse_position_type, bool
                                paused);

        void renderHandling(SDL_Renderer *renderer, const RenderingContext &renderingContext);

        void eventHandling(const SDL_Event &event, const GameEvent::GameEventContext &event_context, bool paused);

        void ProcessDeletionQueues();


        /// <summary>
        /// adds one or more entries to the entities map
        /// </summary>
        void registerEntities(const std::vector<Entity *> &entities);

        /// <summary>
        /// removes one or more entries from the entities map. Used for when the
        /// object is destroyed or if another ship undocks prolly idk
        /// </summary>
        /// <param name="entities"> the entities to unregister</param>
        /// <returns></returns>
        void unregisterEntities(const std::vector<Entity *> &entities, bool delete_when_done);

        bool has_entity(Entity *entity) const;

        Instances::Instance *finalizeRendering(const EntityRendering::Context &entity_rendering_context);

        static std::vector<Entity *> entitiesFromJson(
            const nlohmann::basic_json<> &json,
            GameState::transientGameState &transient_game_state,
            EntityRendering::Context *entity_rendering_context)
        {
            std::vector<Entity *> loaded_entities;
            for (const auto &entity_entry: json) {
                loaded_entities.
                        push_back(EntityLoading::fromJson(entity_entry, transient_game_state,
                                                          entity_rendering_context));
            }
            return loaded_entities;
        }

        enum class RoomDistance {
            All,
            Immediate,
            Close,
            Far,
        };

        /// <summary>
        /// returns the entities (read only)
        /// </summary>
        /// <returns></returns>
        std::vector<Entity *> getEntities() const;

        /// <summary>
        /// returns the physics entities (read only)
        /// </summary>
        /// <returns></returns>
        std::vector<PhysicsEntity *> getPhysicsEntities() const;

        /// <summary>
        /// docks an other ship to this ship. You will want to align them relatively
        /// well since this will snap the other ship. Undocking is future me's problem
        /// </summary>

        std::vector<BehavioredEntity *> getBehavioredEntities() const;

        nlohmann::json entitiesToJson() {

            std::vector<nlohmann::json> entity_entries = nlohmann::json::array();
            for (auto e: entities) {
                if (e->isJsonSerializable()) {
                    entity_entries.push_back(e->toJson());
                }
            }

            return entity_entries;
        }
    };
}
