//
// Created by timon on 3/3/26.
//

#include "TextButton.h"

#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

#include "gameEvent/GameEvent.h"
#include "userInterface/fonts.h"

void GUI::TextButton::render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const {
    //bg
    SDL_FRect bgRect;
    bgRect.w = static_cast<float>(dimensions.x);
    bgRect.h = static_cast<float>(dimensions.y);
    bgRect.x = screenPosition.x;
    bgRect.y = screenPosition.y;

    if (highlighted)SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
    else SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    //text
    SDL_FRect textRect;
    SDL_Color text_color = (highlighted) ? SDL_Color{255, 255, 255, 255} : SDL_Color{255, 255, 200, 255};

    SDL_Surface *surface = TTF_RenderText_Solid(fonts["sm"], text.c_str(), text.length(), text_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    textRect.w = static_cast<float>(surface->w);
    textRect.h = static_cast<float>(surface->h);
    textRect.x = screenPosition.x + (dimensions.x - textRect.w) / 2; // center horizontally
    textRect.y = screenPosition.y + (dimensions.y - textRect.h) / 2; // center vertically

    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void GUI::TextButton::update(const GUI_UpdateContext &context) {
    if (!is_clickable())return;

    GUIRect::update(context);
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    highlighted = context.element_under_mouse == this
                  && (mouse_x >= screenPosition.x && mouse_x <= screenPosition.x + dimensions.x)
                  && (mouse_y >= screenPosition.y && mouse_y <= screenPosition.y + dimensions.y);
}

void GUI::TextButton::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN || context.element_under_mouse != this)
        return;

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);


    if ((mouse_x >= screenPosition.x && mouse_x <= screenPosition.x + dimensions.x)
        &&
        (mouse_y >= screenPosition.y && mouse_y <= screenPosition.y + dimensions.y))
        on_pressed.emit();
}
