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



    MenuNavigation::Navigation SavePickerPage(SDL_Renderer *renderer, SDL_Window *window, std::filesystem::path *save_to_load);

    std::vector<SavedGameEntry> GetSaves(const std::filesystem::path &saves_folder);
};