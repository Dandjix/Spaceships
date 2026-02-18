//
// Created by timon on 2/17/26.
//

#pragma once
#include "physics/scripts/PhysicsEntity.h"
#include "scripts/PhysicsConstraint.h"


namespace Physics::Constraints {
    class Rail : public Constraint {
    protected:
        Vector2Int start;
        Vector2Int end;

        [[nodiscard]] static Vector2Int toWorldPosition
        (Vector2Int owner_position, float owner_angle, Vector2Int relative_position);

    public:
        /**
         * this has to be a world position kinda obviously
         * @return
         */
        [[nodiscard]] Vector2Int getStart() const { return start; }
        /**
         * this has to be a world position kinda obviously
         * @return
         */
        [[nodiscard]] Vector2Int getEnd() const { return end; }
        [[nodiscard]] int getLength() const { return static_cast<int>(std::round((end - start).length())); }


        [[nodiscard]] static Vector2Int computePosition(Vector2Int rail_start, Vector2Int rail_end, float progress);

        [[nodiscard]] static float computeAngleAroundReactor(Vector2Int rail_start, Vector2Int rail_end);

        [[nodiscard]] static float computeRodEntityAngle(Vector2Int start, Vector2Int end) {return - computeAngleAroundReactor(start,end);}
        Rail(Vector2Int start, Vector2Int end);

        void apply(PhysicsEntity *owner_entity) override;

        /**
         *
         * @param owner_entity the entity this belongs to
         * @return 0 if the rail is at the start position, 1 if at the other end, 0.5 in the middle etc.
         */
        float progress(const PhysicsEntity *owner_entity) const;

        void debugRender(PhysicsEntity *owner, SDL_Renderer *renderer, const RenderingContext &context) override;
    };
}
