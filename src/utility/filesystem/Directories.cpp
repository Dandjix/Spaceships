//
// Created by earthberry on 5/30/26.
//

#include "Directories.h"

#include <iostream>

Directories* Directories::instance = nullptr;

Directories * Directories::get() {
    if (instance == nullptr)
        throw DirectoriesNotInitializedException();
    return instance;
}

Directories::Directories(const std::filesystem::path &relative_path_to_executable, std::filesystem::path saves_root):
    relative_path_to_project_root(relative_path_to_executable.parent_path()),
    saves_root(saves_root) {

    std::filesystem::create_directories(saves_root);
}

void Directories::print() const {
    std::cout << "saves path is : " << saves_root << ", assets is at : " << assets() << "(" << relative_path_to_project_root << ")" << std::endl;
}

void Directories::initialize(Directories *directories) {
    if (instance != nullptr)
        throw DirectoriesAlreadyInitializedException();
    instance = directories;
}
