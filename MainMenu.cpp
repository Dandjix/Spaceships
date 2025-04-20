#include "MainMenu.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "MenuNavigation.h"



MenuNavigation RunMainMenu(SDL_Renderer* renderer, TTF_Font* font) {
    std::vector<std::string> options = { "Game", "Settings", "Ship Editor", "Quit" };
    int selected = 0;

    bool running = true;
    SDL_Event e;

    SDL_Color normalColor = { 255, 255, 255, 255 }; // white
    SDL_Color selectedColor = { 255, 255, 0, 255 }; // yellow

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                return Quit;
            }
            if (e.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = e.key.key;

                switch (key) {
                case SDLK_UP:
                    // Move up the menu
                    if (selected > 0) {
                        --selected;
                    }
                    break;

                case SDLK_DOWN:
                    // Move down the menu
                    if (selected < options.size() - 1) {
                        ++selected;
                    }
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    // Enter the selected option
                    switch (selected) {
                    case 0:  // "Game"
                        return Game;
                    case 1:  // "Settings"
                        return Settings;
                    case 2:  // "Ship Editor"
                        return ShipEditor;
                    case 3:  // "Quit"
                        return Quit;
                    }
                    break;
                }
            }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < options.size(); ++i) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), options[i].length(), i == selected ? selectedColor : normalColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            float textW = 0, textH = 0;
            SDL_GetTextureSize(texture, &textW, &textH);
            SDL_FRect dstRect = { 100.0f, 100.0f + i * 60.0f, static_cast<float>(textW), static_cast<float>(textH) };
            SDL_RenderTexture(renderer, texture, NULL, &dstRect);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }

    return Quit;
}