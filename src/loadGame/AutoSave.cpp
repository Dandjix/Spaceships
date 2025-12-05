//
// Created by timon on 12/3/25.
//

#include "AutoSave.h"

#include <format>
#include <iostream>
#include <limits.h>

std::vector<std::string> Saves::splitString(const std::string &to_split, const std::string &delimiter) {
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

bool Saves::isNumber(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool Saves::isAutosave(std::filesystem::path const &pathToAutosave) {
    auto parts = splitString(pathToAutosave.filename(),"_");
    if (parts.size()!=2) {
        return false;
    }
    if (parts[0] != "autosave") {
        return false;
    }
    auto extension_parts = splitString(parts[1],".");
    if (!isNumber(extension_parts[0])) {
        return false;
    }
    return true;
}

unsigned long Saves::getAutoSaveNumber(const std::filesystem::path &path_to_autosave) {
    const std::string & file_name = path_to_autosave.filename();
    auto filename_parts = splitString(file_name,"_");
    unsigned long autosave_number = std::stoul( splitString(filename_parts[1],".")[0]);
    return  autosave_number;
}

unsigned long Saves::getNextAutosaveNumber(const std::filesystem::path &saves_folder) {
    unsigned long greatest_autosave_number = 0;
    for (const auto& file: std::filesystem::directory_iterator(saves_folder)) {
        if (!file.is_regular_file())
            continue;
        if (isAutosave(file.path())) {
            auto autosave_number = getAutoSaveNumber(file.path());
            if (autosave_number > greatest_autosave_number) {
                greatest_autosave_number = autosave_number;
            }
        }
    }
    return greatest_autosave_number + 1;
}

unsigned long count_autosaves(const std::filesystem::path &saves_path) {
    unsigned long autosave_count = 0;
    for (const auto& entry: std::filesystem::directory_iterator(saves_path)) {
        if (Saves::isAutosave(entry.path())) {
            autosave_count ++;
        }
    }
    return autosave_count;
}

void deleteOldestAutosave(std::filesystem::path::iterator::reference saves_path) {
    unsigned long oldest_number = ULONG_MAX;
    std::filesystem::path path_to_oldest;
    for (const auto & entry: std::filesystem::directory_iterator(saves_path)) {
        if (Saves::isAutosave(entry.path())) {
            auto nb = Saves::getAutoSaveNumber(entry.path());
            if (nb <= oldest_number) {
                oldest_number = nb;
                path_to_oldest = entry.path();
            }
        }
    }
    if (path_to_oldest.empty()) {
        throw std::runtime_error(std::format("Could not find an autosave among {}",saves_path.c_str()));
    }
    std::filesystem::remove(path_to_oldest);
}


std::filesystem::path Saves::getNewAutosavePath(const std::filesystem::path &saves_folder) {

    auto number = getNextAutosaveNumber(saves_folder);

    return saves_folder/ ("autosave_"+std::to_string(number)+".save.json");
}

unsigned long Saves::deleteOldAutosaves(const std::filesystem::path &saves_path, unsigned long max_number_auto_saves) {
    unsigned long deleted = 0;

    while (count_autosaves(saves_path) > max_number_auto_saves) {
        deleteOldestAutosave(saves_path);
        deleted ++;
    }
    return deleted;
}

std::filesystem::path Saves::pathToLatestAutoSave(const std::filesystem::path &saves_folder) {
    unsigned long greatest_autosave_number = 0;
    std::string latest_autosave;
    for (const auto& file: std::filesystem::directory_iterator(saves_folder)) {
        if (!file.is_regular_file())
            continue;
        const std::filesystem::path& save_name = file.path().filename();
        if (isAutosave(save_name)) {
            auto filename_parts = splitString(file.path().filename(),"_");
            unsigned long autosave_number = std::stoul( splitString(filename_parts[1],".")[0]);
            if (autosave_number >= greatest_autosave_number) {
                greatest_autosave_number = autosave_number;
                latest_autosave = file.path();
            }
        }
    }
    return latest_autosave;
}
