//
// Created by timon on 7/9/25.
//

#include "Humanoid.h"

#include "physics/Physics.h"

void Humanoid::update(const UpdateContext &context) {
    RoundEntity::update(context);
}

void Humanoid::handleEvent(const SDL_Event &event) {
    RoundEntity::handleEvent(event);
}

void Humanoid::render(SDL_Renderer *renderer, const RenderingContext &context) {

    renderTexture(renderer,context,texture,Vector2Float(
        radius,
        radius
        )
    );
}

const float cushion = 0.1f; // 1/10th of a pixel

void Humanoid::movePosition(Vector2Float delta, SpaceShip * space_ship)
{
    float world_space_radius = static_cast<float>(radius * Vectors::getFactor());

    Vector2Float cast_h = { (delta.x > 0 ? 1.0f : -1.0f) * world_space_radius, 0 };
    float max_h = std::abs(delta.x) + std::abs(world_space_radius);

    Vector2Float cast_v = { 0, (delta.y > 0 ? 1.0f : -1.0f)*world_space_radius };
    float max_v = std::abs(delta.y) + std::abs(world_space_radius);

    auto hit_h = Physics::RayCast(position, cast_h, space_ship, max_h);
    auto hit_v = Physics::RayCast(position, cast_v, space_ship, max_v);

    Vector2Float new_delta = delta;

    if (hit_h)
    {
        float sign = (delta.x > 0 ? 1.f : -1.f);
        float radius_diff = sign * (world_space_radius + cushion);

        float dist = static_cast<float>(hit_h->x - position.x);
        new_delta.x = dist - radius_diff;
    }

    if (hit_v)
    {
        float sign = (delta.y > 0 ? 1.f : -1.f);
        float radius_diff = sign * (world_space_radius + cushion);

        float dist = static_cast<float>(hit_v->y - position.y);
        new_delta.y = dist - radius_diff;
    }

    Entity::movePosition(new_delta, space_ship);
}
