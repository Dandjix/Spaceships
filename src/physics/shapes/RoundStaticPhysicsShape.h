//
// Created by timon on 12/3/25.
//

#pragma once
#include "PhysicsShape.h"
#include "RoundPhysicsShape.h"


class RoundStaticPhysicsShape : public RoundPhysicsShape{
public:

    explicit RoundStaticPhysicsShape(PhysicsEntity *owner_entity, int radius)
        : RoundPhysicsShape(owner_entity,radius) {
    }

    ~RoundStaticPhysicsShape() override = default;

    PhysicsUpdateVisitor * createVisitor() override;

    void consumeVisitor(PhysicsUpdateVisitor *visitor, Instances::Instance *instance) override {
        visitor->visitStaticRound(this,instance);
    }
};
