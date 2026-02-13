#pragma once
#include "physics/scripts/ShortLivedPhysicsEntity.h"
//
// Created by timon on 2/8/26.
//


class DoorPanel : public ShortLivedPhysicsEntity{
protected:
    static SDL_Texture * door_texture;
    Vector2Int dimensions;

public:
    DoorPanel(const Vector2Int &position, const std::optional<float> &angle, Vector2Int dimensions);

    void registerInSpaceship(SpaceShip *space_ship) override;

    DoorPanel * initializeRendering(const EntityRendering::Context &context) override;

    DoorPanel * finalizeRendering(const EntityRendering::Context &context) override;

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    QueueOrder::Value getQueueOrder() override {return QueueOrder::LAST - 1000;}
};