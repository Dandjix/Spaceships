# FetchContent is used to download SDL3 if it's not installed locally
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.4.8
)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG        release-3.2.4
)

FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG        release-3.2.2
)

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf)

include_directories(
        INTERFACE
        ${SDL3_INCLUDE_DIRS}
        ../src/.
        ../src/imported
)

# Collect test files
file(GLOB_RECURSE TEST_SOURCES *.cpp)

add_library(GTest::GTest INTERFACE IMPORTED
        LoadGame/LoadGameTests.cpp)

add_executable(SpaceshipsTests ${TEST_SOURCES})

target_link_libraries(SpaceshipsTests
        PRIVATE
        SpaceshipsLib
        gtest
        gtest_main
)

target_link_libraries(
        SpaceshipsTests
        PRIVATE
        GTest::GTest
)

add_custom_command(TARGET SpaceshipsTests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:SpaceshipsTests>/assets/)