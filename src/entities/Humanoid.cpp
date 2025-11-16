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

void Humanoid::movePosition(Vector2Float delta, SpaceShip * space_ship)
{
    auto point_on_rim = position + Vectors::toVector2Int(delta.normalized()*radius*Vectors::getFactor());

    auto cast_direction = delta.normalized();

    auto hit = Physics::RayCast(point_on_rim,cast_direction,space_ship,delta.length());

    if (!hit.has_value())
    {
        Entity::movePosition(delta,space_ship);
    }
}