//
// Created by timon on 11/27/25.
//

#include "GUITextPrompt.h"

#include "userInterface/fonts.h"

#include <SDL3_ttf/SDL_ttf.h>

void GUITextPrompt::setFocused(bool new_focused) {
    if (new_focused != focused) {
        if (new_focused) {
            SDL_StartTextInput(window);
        }
        else {
            SDL_StopTextInput(window);
        }
        on_focused_change.emit(new_focused);
    }
    focused = new_focused;
}

void GUITextPrompt::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (frames_until_active > 0) {
        return;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        auto new_focused =  is_inside({mouse_x,mouse_y});

        setFocused(new_focused);
    }

    if (!shown ) {
        return;
    }

    if (focused) {
        if (event.type == SDL_EVENT_TEXT_INPUT) {
            value.append(event.text.text);
        }
        if ((event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_BACKSPACE || event.key.key == SDLK_DELETE) && !value.empty()) {
            value.pop_back();
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN) {
            on_confirm.emit(value);
            hide();
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
            setFocused(false);
        }
    }
}

void GUITextPrompt::show() {
    shown = true;
    frames_until_active = 1;
}

void GUITextPrompt::hide() {
    shown = false;
    focused = false;
    value = "";
}

void GUITextPrompt::render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const {
    if (!shown)
        return;

    auto rect = SDL_FRect{
        static_cast<float>(screenPosition.x),static_cast<float>(screenPosition.y),
        static_cast<float>(dimensions.x),static_cast<float>(dimensions.y)
    };

    if (focused) {
        SDL_SetRenderDrawColor(renderer,125,125,125,255);
    }
    else {
        SDL_SetRenderDrawColor(renderer,55,55,55,255);
    }

    SDL_RenderFillRect(renderer,&rect);

    SDL_Color text_color;
    if (focused) {
        text_color = SDL_Color(255,255,255,255);
    }else {
        text_color = SDL_Color(200,255,255,255);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(fonts["md"], value.c_str(), value.length(), text_color);
    if (surface) {
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
}

void GUITextPrompt::update(const GUI_UpdateContext &context) {
    GUIRect::update(context);

    frames_until_active = frames_until_active -1;
    if (frames_until_active < 0)
        frames_until_active = 0;

}
