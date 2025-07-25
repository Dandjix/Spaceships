cmake_minimum_required(VERSION 3.24)
project(Spaceships LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 23)

# FetchContent is used to download SDL3 if it's not installed locally
include(FetchContent)

# Fetch SDL3 if not found locally
find_package(SDL3 QUIET CONFIG REQUIRED)
if(NOT SDL3_FOUND)
    message(STATUS "Fetching SDL3 from GitHub...")
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.18
    )
    FetchContent_MakeAvailable(SDL3)
endif()

find_package(SDL3_image QUIET CONFIG REQUIRED)
if(NOT SDL3_image_FOUND)
    # SDL3_image
    FetchContent_Declare(
            SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG        main  # Or a specific release when available
    )
    FetchContent_MakeAvailable(SDL3_image)
endif()

find_package(SDL3_ttf QUIET CONFIG REQUIRED)
if(NOT SDL3_ttf_FOUND)
    # SDL3_ttf
    FetchContent_Declare(
            SDL3_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG        main  # Or a specific release when available
    )
    FetchContent_MakeAvailable(SDL3_ttf)
endif ()

# Include headers
include_directories(
        ${SDL3_INCLUDE_DIRS}
        src
        src/imported
)

# Collect all source files recursively
file(GLOB_RECURSE SOURCES
        CONFIGURE_DEPENDS
        src/*.cpp
        src/*.c
)

# Create the executable from all your sources
add_executable(${PROJECT_NAME} ${SOURCES}
        src/Environment.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

# SDL3 may provide an imported target (modern usage)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

# If you're using SDL_main and need to override the entry point
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_USE_CALLBACKS)

# On Windows, copy SDL3.dll to the executable output folder after build
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

target_compile_definitions(Spaceships PRIVATE
        ENV_PROJECT_ROOT="${CMAKE_SOURCE_DIR}/"
)