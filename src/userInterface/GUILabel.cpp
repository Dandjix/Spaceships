//
// Created by timon on 11/15/25.
//

#include "GUILabel.h"

void GUILabel::render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect textRect;
    textRect.w = static_cast<float>(surface->w);
    textRect.h = static_cast<float>(surface->h);
    textRect.x = screenPosition.x + (dimensions.x - textRect.w) / 2; // center horizontally
    textRect.y = screenPosition.y + (dimensions.y - textRect.h) / 2; // center vertically

    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}
