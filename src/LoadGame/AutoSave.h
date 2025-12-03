//
// Created by timon on 12/3/25.
//

#pragma once
#include <filesystem>
#include <vector>

//to compile for tests
#ifndef ENV_PROJECT_ROOT
#define ENV_PROJECT_ROOT ""
#endif




namespace Saves {

    inline std::vector<std::string> splitString(const std::string & to_split, const std::string &delimiter = "_") {
        std::vector<std::string> parts = {};
        std::string to_search = to_split;

        bool stop = false;
        while (!stop) {
            unsigned long found_index = to_search.find(delimiter);
            auto to_add = to_search.substr(0,found_index);
            parts.push_back(to_add);
            if (to_search.find(delimiter) == std::string::npos)
                stop = true;
            to_search = to_search.substr(found_index+1);
        }
        return parts;
    }

    // Source - https://stackoverflow.com/a
    // Posted by Charles Salvia, modified by community. See post 'Timeline' for change history
    // Retrieved 2025-12-03, License - CC BY-SA 4.0
    inline bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    inline bool isAutosave (std::string const &fullString) {
        auto parts = splitString(fullString,"_");
        if (parts.size()!=2) {
            return false;
        }
        if (parts[0] != "autosave") {
            return false;
        }
        auto extension_parts = splitString(parts[1],".");
        if (!is_number(extension_parts[0])) {
            return false;
        }
        return true;
    }
    inline unsigned long getNextAutosaveNumber(const std::filesystem::path  &saves_folder) {
        unsigned long greatest_autosave_number = 0;
        for (auto file: std::filesystem::directory_iterator(saves_folder)) {
            if (!file.is_regular_file())
                continue;
            if (isAutosave(file.path().filename())) {
                auto filename_parts = splitString(file.path().filename(),"_");
                unsigned long autosave_number = std::stoul( splitString(filename_parts[1],".")[0]);
                if (autosave_number > greatest_autosave_number) {
                    greatest_autosave_number = autosave_number;
                }
            }
        }
        return greatest_autosave_number + 1;
    }

    std::filesystem::path getSavesPath() {
        return ENV_PROJECT_ROOT"saves";
    }

    inline std::filesystem::path newAutoSavePath(const std::filesystem::path &saves_folder) {

        auto number = getNextAutosaveNumber(saves_folder);

        return "autosave_"+std::to_string(number)+".save.json";
    }
}
