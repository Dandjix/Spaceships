#pragma once
#include "physics/constraints/RailConstraint.h"
#include "physics/constraints/scripts/PhysicsConstraint.h"
#include "physics/scripts/PhysicsEntity.h"
#include "physics/scripts/ShortLivedPhysicsEntity.h"
#include "physics/shapes/PhysicsShape.h"
#include "physics/shapes/RectPhysicsShape.h"
//
// Created by timon on 2/17/26.
//

namespace Contraptions::FissionReactor {
    class ControlRod : public ShortLivedPhysicsEntity {
    protected:
        Physics::Constraints::Rail rail_constraint;

        [[nodiscard]] std::vector<Physics::Constraint *> getConstraints() override {
            return {&rail_constraint};
        }

        PhysicsShape *constructPhysicsShape(Vector2Int dimensions) {
            int tip_length = dimensions.y;

            std::vector<Vector2Int> points = {
                {dimensions.x / 2, -dimensions.y / 2},
                {dimensions.x / 2, dimensions.y / 2},
                {-dimensions.x / 2 + tip_length / 2, dimensions.y / 2},
                {-dimensions.x / 2 - tip_length / 2, 0},
                {-dimensions.x / 2 + tip_length / 2, -dimensions.y / 2},
            };

            return new ConvexPhysicsShape(this, points);
        }

    public:
        ControlRod(Vector2Int rail_start, Vector2Int rail_end, float progress, Vector2Int dimensions)
            : ShortLivedPhysicsEntity(
                  Physics::Constraints::Rail::computePosition(rail_start, rail_end, progress),
                  Physics::Constraints::Rail::computeRodEntityAngle(rail_start, rail_end),
                    //inverted since because it is PLACED at
                    //45 deg clockwise of the reactor, which means
                    //the angle of the actual entity is reversed
                  constructPhysicsShape(dimensions)
              ),
              rail_constraint(rail_start, rail_end) {
        }

        ControlRod(Vector2Int position, float angle, Vector2Int rail_start, Vector2Int rail_end, Vector2Int dimensions)
            : ShortLivedPhysicsEntity(
                  position,
                  angle,
                  constructPhysicsShape(dimensions)
              ),
              rail_constraint(rail_start, rail_end) {
        }

        float getProgress() {
            return rail_constraint.progress(this);
        }

        ControlRod *initializeRendering(const EntityRendering::Context &context) override { return this; }

        ControlRod *finalizeRendering(const EntityRendering::Context &context) override { return this; }

        void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        }

        float computeAngle() {
            return Physics::Constraints::Rail::computeAngleAroundReactor(rail_constraint.getStart(), rail_constraint.getEnd());
        }
    };
}
