//
// Created by timon on 11/28/25.
//

#include "LoadSavedGame.h"

#include <fstream>
#include <SDL3/SDL_timer.h>
#include <map>
#include "gameEvent/GameEvent.h"
#include "userInterface/GUI.h"
#include "userInterface/elements/GUI/GUIList.h"


MenuNavigation::Navigation LoadSavedGame::SavePickerPage(SDL_Renderer *renderer, SDL_Window *window, std::filesystem::path *save_to_load) {

    MenuNavigation::Navigation navigation = MenuNavigation::LoadGame;

    auto saves = GetSaves(ENV_PROJECT_ROOT"saves");

    std::map<std::string,std::string> option_map = {

    };
    for (const auto& save: saves) {
        option_map.insert({save.save_name,save.path});
    }
    option_map.insert({"Back to Main Menu","back"});

    std::vector<std::string> option_keys = {};
    for (const auto& key: option_map | std::views::keys) {
        option_keys.push_back(key);
    }

    auto menu = GUIList(Anchor::Center, {0, 0}, GUI_Fill, GUI_Fill, option_keys, [&option_map,&navigation,save_to_load](const std::string &key) {
        const std::string & selected = option_map.at(key);
        if (selected == "back") {
            navigation = {MenuNavigation::MainMenu};
            return;
        }
        const std::string & save_path = option_map.at(key);
        navigation = MenuNavigation::Game;
        *save_to_load = std::filesystem::path(save_path);
    }, true);

    while (navigation == MenuNavigation::LoadGame) {
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

std::string completeExtension(const std::filesystem::path &path) {

    std::string filename = path.filename();

    auto found = filename.find('.');
    if (found == std::string::npos)
        return "";

    return filename.substr(found,std::string::npos);
}

std::string completeFilename(const std::filesystem::path &path) {

    std::string filename = path.filename();

    auto found = filename.find('.');
    if (found == std::string::npos)
        return "";

    return filename.substr(0,found);
}

std::vector<LoadSavedGame::SavedGameEntry> LoadSavedGame::GetSaves(const std::filesystem::path &saves_folder) {

    auto entries = std::vector<SavedGameEntry>();

    for (const auto & e : std::filesystem::directory_iterator(saves_folder)) {
        auto path = e.path();
        if (completeExtension(path) == ".save.json") {
            entries.push_back({
                completeFilename(path),
                path,
                0
            });
        }
    }

    return entries;
}
