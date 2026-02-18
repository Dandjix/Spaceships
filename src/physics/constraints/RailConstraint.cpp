//
// Created by timon on 2/17/26.
//
#include "RailConstraint.h"

#include "physics/scripts/PhysicsEntity.h"


namespace Physics::Constraints {
    Vector2Int Rail::toWorldPosition(Vector2Int owner_position, float owner_angle, Vector2Int relative_position) {
        Vector2Int rotated = relative_position.rotate(owner_angle);
        Vector2Int translated = owner_position + rotated;
        return translated;
    }

    Vector2Int Rail::computePosition(Vector2Int rail_start, Vector2Int rail_end, float progress) {
        Vector2Float direction = Vectors::toVector2Float(rail_end - rail_start).normalized();

        float length = (rail_end - rail_start).length();

        Vector2Int position = rail_start + Vectors::toVector2Int(direction * progress * length);

        return position;
    }

    float Rail::computeAngle(Vector2Int rail_start, Vector2Int rail_end) {
        return (rail_end - rail_start).angle_to({1, 0});
    }

    Rail::Rail(Vector2Int start, Vector2Int end) : start(start), end(end) {
    }

    void Rail::apply(PhysicsEntity *owner_entity) {
        //https://stackoverflow.com/questions/64330618/finding-the-projection-of-a-point-onto-a-line

        Vector2Int A = getStart();
        Vector2Int B = getEnd();
        Vector2Int C = owner_entity->getPosition();

        int abx = B.x - A.x;
        int aby = B.y - A.y;
        int acx = C.x - A.x;
        int acy = C.y - A.y;

        float progress = static_cast<float>(abx * acx + aby * acy) / static_cast<float>(abx * abx + aby * aby);

        progress = std::clamp(progress, 0.0f, 1.0f);

        int dx = A.x + static_cast<int>(static_cast<float>(abx) * progress);
        int dy = A.y + static_cast<int>(static_cast<float>(aby) * progress);

        owner_entity->setPosition({dx, dy});
    }

    [[nodiscard]] float Rail::progress(const PhysicsEntity *owner_entity) const {
        Vector2Int A = getStart();
        Vector2Int B = getEnd();
        Vector2Int C = owner_entity->getPosition();

        int abx = B.x - A.x;
        int aby = B.y - A.y;
        int acx = C.x - A.x;
        int acy = C.y - A.y;

        float progress = static_cast<float>(abx * acx + aby * acy) / static_cast<float>(abx * abx + aby * aby);

        return progress;
    }

    void Rail::debugRender(PhysicsEntity *owner, SDL_Renderer *renderer, const RenderingContext &context) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        Vector2Float start_screen = context.camera_info.worldToScreenPoint(getStart());
        Vector2Float end_screen = context.camera_info.worldToScreenPoint(getEnd());

        SDL_RenderLine(renderer, start_screen.x, start_screen.y, end_screen.x, end_screen.y);
    }
}
