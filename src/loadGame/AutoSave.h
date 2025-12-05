//
// Created by timon on 12/3/25.
//

#pragma once
#include <filesystem>
#include <vector>
#include <SDL3/SDL_log.h>

//to compile for tests
#ifndef ENV_PROJECT_ROOT
#define ENV_PROJECT_ROOT ""
#endif




namespace Saves {
    std::vector<std::string> splitString(const std::string & to_split, const std::string &delimiter = "_");

    // Source - https://stackoverflow.com/a
    // Posted by Charles Salvia, modified by community. See post 'Timeline' for change history
    // Retrieved 2025-12-03, License - CC BY-SA 4.0
    bool isNumber(const std::string& s);

    bool isAutosave (std::filesystem::path const &pathToAutosave);

    unsigned long getAutoSaveNumber(const std::filesystem::path &path_to_autosave);

    unsigned long getNextAutosaveNumber(const std::filesystem::path  &saves_folder);

    inline std::filesystem::path getSavesPath() {
        return ENV_PROJECT_ROOT"saves";
    }

    [[nodiscard]] std::filesystem::path getNewAutosavePath(const std::filesystem::path &saves_folder = getSavesPath());

    /**
     * deletes the oldest autosaves.
     * @param saves_path the path to the saves folder
     * @param max_number_auto_saves the max number of autosaves allowed, defaults to 16
     * @return the number of saves deleted
     */
    unsigned long deleteOldAutosaves(const std::filesystem::path &saves_path = getSavesPath(),
                                     unsigned long max_number_auto_saves = 16);

    std::filesystem::path pathToLatestAutoSave(const std::filesystem::path &saves_folder = getSavesPath());
}
