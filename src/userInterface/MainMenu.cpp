#include "MainMenu.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include "MenuNavigation.h"
#include "fonts.h"
#include "elements/GUI/GUIList.h"


MenuNavigation::Navigation RunMainMenu(SDL_Renderer *renderer, SDL_Window *window) {
    MenuNavigation::Navigation navigation = MenuNavigation::MainMenu;

    auto menu = GUIList(Anchor::Center, {0, 0}, GUI_Fill, GUI_Fill, {
                            "New Game",
                            "Load Save",
                            "Ship Editor",
                            "Exit to desktop"
                        }, [renderer,window,&navigation](const std::string &selected) {
                            if (selected == "New Game") {
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
            {win_x, win_y},
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
                {0,0},
                0,
                {win_x,win_y}
            },
            GameEvent::UI,
            window
        };

        while (SDL_PollEvent(&event)) {
            menu.handleEvent(event,game_event_context);
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
