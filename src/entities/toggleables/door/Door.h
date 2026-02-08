#pragma once
#include "entities/toggleables/scripts/Toggleable.h"
#include "physics/scripts/PhysicsEntity.h"
#include "spaceships/Tile.h"
#include "spaceships/EntityData/EntityLoading.h"
//
// Created by timon on 2/7/26.
//


// -
class Door : public PhysicsEntity, public virtual Toggleable {
public:
    EntityId::entityId entity_id;
    Vector2Int dimensions;
    float state; //state 0 : open, 1 : closed
    float moment; //(deltaTime * moment) added to state on update (if nothing is in the way of the door closing)

    // SDL_Texture * door_texture;

    Door(
        Vector2Int position,
        float angle,
        float state,
        float moment,
        EntityId::entityId entity_id,
        Vector2Int dimensions = {Scaling::scaleToWorld(Tiles::tileSizePx) * 2, Scaling::scaleToWorld(Tiles::tileSizePx)}
    );

    FROM_JSON_DECLARATION(Door, "door");

    nlohmann::json toJson() override;

    Entity *initializeRendering(const EntityRendering::Context &context) override;

    Entity *finalizeRendering(const EntityRendering::Context &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    //near the end for rendering reasons (on top of most entities)
    QueueOrder::Value getQueueOrder() override { return QueueOrder::LAST - 1000; }

    [[nodiscard]] EntityId::entityId getEntityId() const override { return entity_id; }

    [[nodiscard]] Entity *asEntity() override { return this; }

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

    [[nodiscard]] Toggleable * asToggleable() override {return this;}
};
