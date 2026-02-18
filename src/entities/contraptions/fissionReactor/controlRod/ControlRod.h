#pragma once
#include "physics/constraints/RailConstraint.h"
#include "physics/constraints/scripts/PhysicsConstraint.h"
#include "physics/scripts/PhysicsEntity.h"
#include "physics/shapes/PhysicsShape.h"
#include "physics/shapes/RectPhysicsShape.h"
//
// Created by timon on 2/17/26.
//

namespace Contraptions::FissionReactor {
    class ControlRod : public PhysicsEntity {
    protected:
        Physics::Constraints::Rail rail_constraint;

        [[nodiscard]] std::vector<Physics::Constraint *> getConstraints() override {
            return {&rail_constraint};
        }

        PhysicsShape *constructPhysicsShape() {
            return new RectPhysicsShape(this, Scaling::metricToWorld(Vector2Float(0.45f, 5)));
        }

    public:
        ControlRod(Vector2Int rail_start, Vector2Int rail_end, float progress)
            : PhysicsEntity(
                  Physics::Constraints::Rail::computePosition(rail_start, rail_end, progress),
                  Physics::Constraints::Rail::computeAngle(rail_start, rail_end),
                  constructPhysicsShape()
              ),
              rail_constraint(rail_start, rail_end) {
        }

        ControlRod(Vector2Int position, float angle, Vector2Int rail_start, Vector2Int rail_end)
            : PhysicsEntity(
                  position,
                  angle,
                  constructPhysicsShape()
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

        FROM_JSON_DECLARATION(ControlRod, "fission_reactor_control_rod");

        nlohmann::json toJson() override;
    };
}
