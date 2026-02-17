#pragma once
#include "../entities/scripts/Entity.h"
#include "../math/Vectors.h"
#include "../game/Rendering.h"
#include "entities/scripts/LateUpdateEntity.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/EntityData/EntityLoading.h"

class Camera : public LateUpdateEntity {  // Inherit from Entity
protected:
    Entity* player;  // Pointer to the player entity
    Vector2Int screenDimensions = Vector2Int(0, 0);

    Events::Id player_ownership_changed_id = Events::null_id;
private :
    float scale;
public:

    /// <summary>
    /// Creates a Camera that follows the player.
    /// </summary>
    /// <param name="p">Pointer to the player entity</param>
    Camera(Vector2Int position, float angle, float scale);

    ~Camera() override;

    Instances::Instance * working_instance = nullptr;

    /// <summary>
    /// Set the player entity that the camera follows.
    /// </summary>
    /// <param name="p">Pointer to the new player entity</param>
    void setPlayer(Entity* p);

    void handleEvent(const SDL_Event& event, const GameEvent::GameEventContext &context) override;

    QueueOrder::Value getQueueOrder() override;

    void setScale(float s);

    float getScale() const;

    void setScreenDimensions(Vector2Int value);


    /// <summary>
    /// A camera renders nothing.
    /// </summary>
    void render(SDL_Renderer* renderer,const RenderingContext& context) override;

    /// <summary>
    /// Update the camera's position to follow the player.
    /// </summary>
    void update(const UpdateContext & context) override;

    void lateUpdate(const UpdateContext &context) override;

    nlohmann::json toJson() override;

    FROM_JSON_DECLARATION(Camera, "camera");

    void registerInInstance(Instances::Instance *world_instance) override;

    void unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) override;

    void onRegistered(Instances::Instance *newInstance) override;

    Entity * initializeRendering(const EntityRendering::Context &context) override;

    Entity * finalizeRendering(const EntityRendering::Context &context) override;
};
