#pragma once
#include "entities/toggleables/scripts/Toggleable.h"
#include "physics/scripts/PhysicsEntity.h"
#include "spaceships/Tile.h"
#include "spaceships/EntityData/EntityLoading.h"
#include "math/Scaling.h"
//
// Created by timon on 2/7/26.
//


class RectStaticPhysicsShape;

// -
class Door : public Entity, public virtual Toggleable {
protected:
    PhysicsEntity * door_left;
    PhysicsEntity * door_right;

    RectStaticPhysicsShape * getDoorLeftShape();
    RectStaticPhysicsShape * getDoorRightShape();

    [[nodiscard]] Vector2Int getDoorPosition(bool right) const;
    [[nodiscard]] float getDoorAngle(bool right) const;
    [[nodiscard]] Vector2Int getDoorDimensions(bool right) const;

    static SDL_Texture * floor_texture;
public:
    EntityId::entityId entity_id;
    Vector2Int dimensions;
    /**
     * 0 : open, 1 : closed
     */
    float state;
    /**
     * (deltaTime * moment) added to state on update (if nothing is in the way of the door closing)
     */
    float moment;


    Door(
        Vector2Int position,
        float angle,
        float state,
        float moment,
        EntityId::entityId entity_id,
        Vector2Int dimensions = {Scaling::scaleToWorld(Tiles::tileSizePx) * 2, Scaling::scaleToWorld(Tiles::tileSizePx)}
    );

    ~Door() override;

    [[nodiscard]] Vector2Int getCastPosition(bool left) const;

    [[nodiscard]] Vector2Int getCastDimensions(bool left) const;

    [[nodiscard]] bool hasOtherEntity(std::vector<PhysicsShape *> cast_res_1) const;

    void update(const UpdateContext &context) override;

    FROM_JSON_DECLARATION(Door, "door");

    nlohmann::json toJson() override;

    Entity *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    //near the end for rendering reasons (on top of most entities)
    QueueOrder::Value getQueueOrder() override { return QueueOrder::LAST - 1000; }

    [[nodiscard]] EntityId::entityId getEntityId() const override { return entity_id; }

    [[nodiscard]] Entity *asEntity() override { return this; }
    [[nodiscard]] Toggleable * asToggleable() override {return this;}

    void toggle() override {
        if (moment > 0)
            moment = -1;
        else
            moment = 1;
    }


    bool getStatus() override {return moment >0;}

    void setStatus(bool value) override {
        if (value)
            moment = 1;
        else
            moment = -1;
    }


    void registerInSpaceship(SpaceShip *space_ship) override;

    void unregisterInSpaceship(SpaceShip *space_ship, bool delete_when_done) override;

};
