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

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf googletest)

# Include headers
include_directories(
        INTERFACE
        ${SDL3_INCLUDE_DIRS}
        .
        imported
)

# Collect all source files recursively
file(GLOB_RECURSE SOURCES
        CONFIGURE_DEPENDS
        *.cpp
        *.c
)
# library for unit tests
add_library(SpaceshipsLib ${SOURCES})

# Create the executable from all your sources
add_executable(${PROJECT_NAME} ${SOURCES}

)

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

# SDL3 may provide an imported target (modern usage)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)
target_link_libraries(SpaceshipsLib
        PUBLIC
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

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)