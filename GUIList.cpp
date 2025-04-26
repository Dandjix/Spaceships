#include "GUIList.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include "fonts.h"
#include "GUI.h"

const SDL_Color textColor = { 255,255,255,255 };
const SDL_Color highlightedColor = { 255, 255, 0, 255 };
const SDL_Color selectedColor = { 0, 125, 255, 255 };
const SDL_Color selectedHighlightedColor = { 0, 0, 255, 255 };


SDL_FRect GUIList::buttonRect(int i) const
{
    return {
        static_cast<float>(screenPosition.x + outerPadding),
            static_cast<float>(screenPosition.y + outerPadding + (buttonHeight + marginY) * i),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
    };
}


void GUIList::render(SDL_Renderer * renderer, const GUI_RenderingContext& context) const
{
    SDL_FRect bgRect{
    static_cast<float>(screenPosition.x),
    static_cast<float>(screenPosition.y),
    static_cast<float>(dimensions.x),
    static_cast<float>(dimensions.y)
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bgRect);

    //rendering elements

    TTF_Font* font = fonts["sm"];
    SDL_Color tc;

    for (int i = 0; i < options.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

        SDL_FRect rect = buttonRect(i);

        SDL_RenderFillRect(renderer, &rect);

        if (i == highlighted_index && i == selected_index)
        {
            tc = selectedHighlightedColor;
        }
        else if (i == highlighted_index)
        {
            tc = highlightedColor;
        }
        else if (i == selected_index)
        {
            tc = selectedColor;
        }
        else
        {
            tc = textColor;
        }

        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), options[i].length(), tc);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FRect textRect;
        textRect.w = static_cast<float>(surface->w);
        textRect.h = static_cast<float>(surface->h);
        textRect.x = rect.x + (rect.w - textRect.w) / 2; // center horizontally
        textRect.y = rect.y + (rect.h - textRect.h) / 2; // center vertically

        SDL_RenderTexture(renderer, texture, nullptr, &textRect);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }
}

void GUIList::update(const UpdateContext& context) {
    GUIRect::update(context);

    buttonWidth = dimensions.x - outerPadding * 2;

    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (int i = 0; i < options.size(); i++)
    {
        SDL_FRect rect = buttonRect(i);
        Vector2Int tl(rect.x, rect.y);
        Vector2Int br(rect.x + rect.w, rect.y + rect.h);

        if (mouseX >= tl.x && mouseX <= br.x && mouseY >= tl.y && mouseY <= br.y)
        {
            highlighted_index = i;
            break;
        }
    }
}

void GUIList::handleEvent(const SDL_Event event)
{
    if (keyboardNavigation && event.type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Keycode key = event.key.key;

        switch (key)
        {
        case SDLK_UP:
            if (highlighted_index >= 1)
                highlighted_index--;
            break;
        case SDLK_DOWN:
            if (highlighted_index < options.size()-1)
                highlighted_index++;
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            onSelect(options.at(highlighted_index));
            selected_index = highlighted_index;
            break;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (int i = 0; i < options.size(); i++)
        {
            SDL_FRect rect = buttonRect(i);
            Vector2Int tl(rect.x, rect.y);
            Vector2Int br(rect.x + rect.w, rect.y + rect.h);

            if (mouseX >= tl.x && mouseX <= br.x && mouseY >= tl.y && mouseY <= br.y)
            {
                onSelect(options.at(highlighted_index));
                selected_index = highlighted_index;
                break;
            }
        }
    }

}