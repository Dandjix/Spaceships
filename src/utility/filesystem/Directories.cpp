//
// Created by earthberry on 5/30/26.
//

#include "Directories.h"

Directories* Directories::instance = nullptr;

Directories * Directories::get() {
    if (instance == nullptr)
        throw DirectoriesNotInitializedException();
    return instance;
}

void Directories::initialize(Directories *directories) {
    if (instance != nullptr)
        throw DirectoriesAlreadyInitializedException();
    instance = directories;
}
