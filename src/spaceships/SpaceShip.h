#pragma once
#include "../game/Rendering.h"
#include "../math/AdjacencyListGraph.h"
#include "../instance/Rooms/Room.h"
#include <unordered_set>

#include "entities/scripts/Entity.h"
#include "entityContainer/EntityContainer.h"
#include "exterior/SpaceShipExterior.h"
#include "hashProximityList/HashProximityMap.h"
#include "Hooks/SpaceshipHooks.h"
#include "instance/Instance.h"
#include "spaceshipTiles/SpaceshipTiles.h"

namespace EntityId {
    class Manager;
}

class SpaceShipBlueprint;

namespace GameState {
    struct transientGameState;
}

class ActiveWhenPausedEntity;
class BehavioredEntity;
class Camera;
class PhysicsEntity;
class Entity;
class LateUpdateEntity;


class SpaceShip {
protected:
    SpaceShipResources::Exterior *exterior;
    Vector2Int position;

public:
    [[nodiscard]] Vector2Int getPosition() const { return position; }
    void setPosition(Vector2Int value) { position = value; }

    [[nodiscard]] Vector2Int getCenterOffset() const;

    [[nodiscard]] Vector2Int getCenter() const;

protected:
    float angle;

public:
    [[nodiscard]] float getAngle() const { return angle; }

    /**
     * This sets the angle of the ship directly, but if you rotate the ship like this, it is going to get rotated around its top left corner. This is most likely not what you want, see setAngleAroundCenter
     * @param value
     */
    void setAngle(float value) { angle = value; }

    void setAngleAroundCenter(float newAngle) {
        Vector2 center = position + getCenterOffset().rotate(angle);
        angle = newAngle;
        position = center - getCenterOffset().rotate(angle);
    }

public:
    std::filesystem::path blueprint_path;
    /**
     * contains all the entities and stuff
     */
    Instances::Instance *instance;
    SpaceshipTiles *spaceship_tiles;
    SpaceshipRooms *spaceship_rooms;
    SpaceshipHooks *spaceship_hooks;


    explicit SpaceShip(const SpaceShipBlueprint *blueprint,
                       const std::vector<Entity *> &entities,
                       Vector2Int position,
                       float angle);

    explicit SpaceShip();

    ~SpaceShip();

    /// <summary>
    /// renders the hull of the ship
    /// </summary>
    void renderExterior(SDL_Renderer *renderer, const ExteriorRenderingContext &context) const;

    /// <summary>
    /// renders the visible rooms of the ship
    /// </summary>
    void renderInterior(SDL_Renderer *renderer, const RenderingContext &context) const;

    void Dock(SpaceShip other);


    static SpaceShip *fromJson(
        nlohmann::json::const_reference json,
        GameState::transientGameState &identifiedEntities,
        EntityId::Manager &entity_id_manager,
        EntityRendering::Context *entity_rendering_context = nullptr);

    SpaceShip *finalizeRendering(const EntityRendering::Context &entity_rendering_context);

    nlohmann::json toJson();
};
