#pragma once
#include "entities/scripts/ShortLivedEntity.h"
#include "physics/Physics.h"
#include "userInterface/fonts.h"
//
// Created by timon on 2/3/26.
//

namespace Debug {

class PointCastChecker : public ShortLivedEntity {
protected:
    std::vector<PhysicsEntity *> entities_under_mouse;

public:
    PointCastChecker()
        : ShortLivedEntity({0, 0}, 0) {
    }

    PointCastChecker *initializeRendering(const EntityRendering::Context &context) override { return this; }

    PointCastChecker *finalizeRendering(const EntityRendering::Context &context) override { return this; }

    void render(SDL_Renderer *renderer, const RenderingContext &context) override {
        auto font = fonts["lg"];
        std::string text = std::format("there are {} shapes under mouse", entities_under_mouse.size());
        SDL_Color color = {255, 255, 255, 255};

        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        auto world_mouse_position = context.camera_info.screenToWorldPoint({mouse_x, mouse_y});
        Vector2Float screenPosition = context.camera_info.worldToScreenPoint(world_mouse_position);
        Vector2Float dimensions = {100, 100};


        SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FRect textRect;
        textRect.w = static_cast<float>(surface->w);
        textRect.h = static_cast<float>(surface->h);
        textRect.x = screenPosition.x + (dimensions.x - textRect.w) / 2; // center horizontally
        textRect.y = screenPosition.y + (dimensions.y - textRect.h) / 2; // center vertically

        SDL_RenderTexture(renderer, texture, nullptr, &textRect);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }

    void update(const UpdateContext &context) override;
};

inline void PointCastChecker::update(const UpdateContext &context) {
    ShortLivedEntity::update(context);

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    auto world_mouse_position = context.camera_info.screenToWorldPoint({mouse_x, mouse_y});

    entities_under_mouse = Physics::EntityPointCast(world_mouse_position, context.spaceShip);
}
}
