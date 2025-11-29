//
// Created by timon on 11/28/25.
//

#pragma once
#include <filesystem>
#include <string>
#include <utility>
#include <vector>
#include <SDL3/SDL_render.h>
#include <ctime>

#include "spaceships/SpaceShip.h"
#include "userInterface/MenuNavigation.h"


namespace LoadSavedGame {

    struct SavedGameEntry {
        std::string save_name;
        std::filesystem::path path;
        time_t last_date_saved;
    };

    class SavedGameData {
    public:
        explicit SavedGameData(std::filesystem::path save_path) : save_path(std::move(save_path)) {  }

        std::filesystem::path save_path;
        std::vector<SpaceShip *> load_spaceships();
    private:
        static std::vector<SpaceShip *> loads_spaceships(const std::string& from);
    };

    MenuNavigation::Navigation SavePickerPage(SDL_Renderer *renderer, SDL_Window *window, std::filesystem::path *save_to_load);

    std::vector<SavedGameEntry> GetSaves(const std::filesystem::path &saves_folder);
};