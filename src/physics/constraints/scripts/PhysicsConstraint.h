#pragma once
#include "entities/scripts/Entity.h"
//
// Created by timon on 2/17/26.
//

class PhysicsEntity;
struct RenderingContext;

namespace Physics {
    class Constraint {
    public:
        virtual ~Constraint() = default;

        virtual void apply(PhysicsEntity *owner_entity) = 0;

        virtual void debugRender(PhysicsEntity *owner, SDL_Renderer *renderer, const RenderingContext &context) {
        }
    };
}
