//
// Created by earthberry on 5/30/26.
//

#pragma once

#include <filesystem>
#include <utility>



class Directories {
    class DirectoriesNotInitializedException : public std::runtime_error {
    public:
        DirectoriesNotInitializedException()
        : std::runtime_error("You tried accessing the Directories instance before initializing it."
                             " Initialization should be the first thing to be done in the app entry point.")
        {}
    };
    class DirectoriesAlreadyInitializedException : public std::runtime_error {
    public:
        DirectoriesAlreadyInitializedException()
        : std::runtime_error("Instance already initialized")
        {}
    };


private:
    static Directories * instance;
public:
    static  Directories * get();

private:
    const std::filesystem::path relative_path_to_project_root;
    const std::filesystem::path saves_root;

public:
    Directories(
        const std::filesystem::path & relative_path_to_executable,
        std::filesystem::path saves_root
    )
    :
    relative_path_to_project_root(relative_path_to_executable.parent_path()),
    saves_root(std::move(saves_root)) {}

    [[nodiscard]] std::filesystem::path savesRoot() const {
        return saves_root;
    }

    // std::filesystem::path saves(const std::string & save_folder) {
    //
    // }

    [[nodiscard]] std::filesystem::path assets() const {
        return relative_path_to_project_root / "assets";
    }

    void print() const {
        std::cout << "saves path is : " << saves_root << ", assets is at : " << assets() << "(" << relative_path_to_project_root << ")" << std::endl;
    }

    static void initialize(Directories * directories);
};

