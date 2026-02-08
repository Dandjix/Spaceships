#include "MainMenu.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "MenuNavigation.h"
#include "fonts.h"
#include "elements/GUI/GUIList.h"
#include "loadGame/AutoSave.h"


MenuNavigation::Navigation RunMainMenu(SDL_Renderer *renderer, SDL_Window *window) {
    MenuNavigation::Navigation navigation = MenuNavigation::MainMenu;

    std::vector<std::string> options = {};

    {
        auto latest_save = Saves::pathToLatestAutoSave();
        if (!latest_save.empty()) {
            options.emplace_back("Continue Playing");
        }

    }
    options.emplace_back("New Game");
    options.emplace_back("Load Save");
    options.emplace_back("Ship Editor");
    options.emplace_back("Exit to Desktop");


    auto menu = GUIList(Anchor::Center, {0, 0}, GUI_Fill, GUI_Fill, options,
                        [&navigation](const std::string &selected) {
                            if (selected == "Continue Playing") {
                                navigation = MenuNavigation::LoadLatest;
                            } else if (selected == "New Game") {
                                navigation = MenuNavigation::NewGame;
                            } else if (selected == "Load Save") {
                                navigation = MenuNavigation::LoadGame;
                            } else if (selected == "Ship Editor") {
                                navigation = MenuNavigation::ShipEditor;
                            } else if (selected == "Exit to desktop") {
                                navigation = MenuNavigation::Quit;
                            }
                        }, true);

    while (navigation == MenuNavigation::MainMenu) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int win_x, win_y;
        SDL_GetWindowSize(window, &win_x, &win_y);

        GUI_RenderingContext rendering_context = {
            {
                {0, 0},
                0,
                {win_x, win_y},
                1
            }
        };

        menu.render(renderer, rendering_context);

        float delta_time = 16.0f / 1000.0f;

        GUI_UpdateContext update_context = {
            {
                {0, 0},
                0,
                {win_x, win_y}
            },
            delta_time
        };

        SDL_Event event;

        GameEvent::GameEventContext game_event_context = {
            {
                {0, 0},
                0,
                {win_x, win_y}
            },
            nullptr,
            GameEvent::UI,
            window
        };

        while (SDL_PollEvent(&event)) {
            menu.handleEvent(event, game_event_context);
            if (event.type == SDL_EVENT_QUIT) {
                return MenuNavigation::Quit;
            }
        }

        menu.update(update_context);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }
    return navigation;
}
