//
// Created by timon on 11/29/25.
//

#include "GUITooltip.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "userInterface/fonts.h"

void GUITooltip::enable() {
    enabled = true;
}

void GUITooltip::disable() {
    enabled = false;
}

void GUITooltip::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    GUIRect::handleEvent(event, context);
}

void GUITooltip::render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const {
    if (!enabled || text.empty())
        return;

    SDL_Surface* surface = TTF_RenderText_Solid(fonts["sm"], text.c_str(), text.length(), text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect textRect;
    textRect.w = static_cast<float>(surface->w);
    textRect.h = static_cast<float>(surface->h);
    textRect.x = screen_position.x - textRect.w / 2; // center horizontally
    textRect.y = screen_position.y - textRect.h / 2; // center vertically

    SDL_FRect rect = {
        screen_position.x - static_cast<float>(surface->w)*0.5f,
        screen_position.y - static_cast<float>(surface->h)*0.5f,
        static_cast<float>(surface->w),
        static_cast<float>(surface->h)
    };
    SDL_SetRenderDrawColor(renderer,rect_color.r,rect_color.g,rect_color.b,rect_color.a);
    SDL_RenderFillRect(renderer,&rect);
    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}
