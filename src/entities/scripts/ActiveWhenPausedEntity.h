//
// Created by timon on 12/3/25.
//

#pragma once
#include "Entity.h"
#include "ShortLivedEntity.h"


class ActiveWhenPausedEntity : public ShortLivedEntity{
public:
    ActiveWhenPausedEntity(const Vector2Int &position, const std::optional<float> &angle)
        : ShortLivedEntity(position, angle) {
    }

    ~ActiveWhenPausedEntity() override = default;

    void registerInInstance(Instances::Instance *world_instance) override;

    void unregisterInInstance(Instances::Instance *world_instance, bool delete_when_done) override;
};

