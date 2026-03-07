//
// Created by timon on 11/27/25.
//

#include "GUIStringPrompt.h"

#include "userInterface/fonts.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "gameEvent/GameEvent.h"

void GUI::Prompts::StringPrompt::setFocused(bool new_focused) {
    if (new_focused != focused) {
        if (new_focused) {
            SDL_StartTextInput(window);
        } else {
            SDL_StopTextInput(window);
        }
        on_focused_change.emit(new_focused);
    }
    focused = new_focused;
}

void GUI::Prompts::StringPrompt::handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) {
    if (frames_until_active > 0) {
        return;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        auto new_focused = context.element_under_mouse == this;

        setFocused(new_focused);
    }

    if (!shown) {
        return;
    }

    if (focused) {
        if (event.type == SDL_EVENT_TEXT_INPUT) {
            value.append(event.text.text);
        }
        if (
            (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_BACKSPACE || event.key.key == SDLK_DELETE)
            && !value.empty()
        ) {
            value.pop_back();
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RETURN) {
            on_confirm.emit(value);
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
            setFocused(false);
        }
    }
}

void GUI::Prompts::StringPrompt::show() {
    shown = true;
    frames_until_active = 1;
}

void GUI::Prompts::StringPrompt::hide() {
    shown = false;
    focused = false;
    value = "";
}

void GUI::Prompts::StringPrompt::render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const {
    if (!shown)
        return;

    auto rect = SDL_FRect{
        static_cast<float>(screenPosition.x), static_cast<float>(screenPosition.y),
        static_cast<float>(dimensions.x), static_cast<float>(dimensions.y)
    };

    if (focused) SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
    else SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);

    SDL_RenderFillRect(renderer, &rect);
    {
        SDL_Color annotation_color = SDL_Color(200, 100, 100, 255);

        if (SDL_Surface *surface = TTF_RenderText_Solid(fonts["sm"], annotation.c_str(), annotation.length(),
                                                        annotation_color)) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_FRect annotationRect;
            annotationRect.w = static_cast<float>(surface->w);
            annotationRect.h = static_cast<float>(surface->h);
            annotationRect.x = screenPosition.x; // top left
            annotationRect.y = screenPosition.y; // top left

            SDL_RenderTexture(renderer, texture, nullptr, &annotationRect);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }
    {
        SDL_Color text_color = SDL_Color(focused ? 255 : 200, 255, 255, 255);

        if (SDL_Surface *surface = TTF_RenderText_Solid(fonts["md"], value.c_str(), value.length(), text_color)) {
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
    }
}

void GUI::Prompts::StringPrompt::update(const GUI_UpdateContext &context) {
    GUIRect::update(context);

    frames_until_active = frames_until_active - 1;
    if (frames_until_active < 0)
        frames_until_active = 0;
}
