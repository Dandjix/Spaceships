
# FetchContent is used to download SDL3 if it's not installed locally
include(FetchContent)

# Fetch SDL3 if not found locally
find_package(SDL3 QUIET CONFIG REQUIRED)


find_package(SDL3_image QUIET CONFIG REQUIRED)


find_package(SDL3_ttf QUIET CONFIG REQUIRED)


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
add_library(SpaceshipsLib ${SOURCES}
        gameEvent/MousePositionType.h
        gameEvent/GetMousePositionType.cpp
        gameEvent/GetMousePositionType.h
        shipEditor/shipEditorModes/ShipEditorStateMachine.h
        shipEditor/shipEditorModes/ModeTilePainter.cpp
        shipEditor/shipEditorModes/ModeTilePainter.h
        shipEditor/shipEditorModes/ModeHookPainter.cpp
        shipEditor/shipEditorModes/ModeHookPainter.h
        shipEditor/shipEditorModes/CommonEditorEntities.h
        shipEditor/HookPainter/HookAppearance.cpp
        shipEditor/HookPainter/HookAppearance.h
        shipEditor/HookPainter/PainterStates/Painter.cpp
        shipEditor/HookPainter/PainterStates/Painter.h
        shipEditor/HookPainter/RegionDeleter.cpp
        shipEditor/HookPainter/RegionDeleter.h
        shipEditor/HookPainter/PointDeleter.cpp
        shipEditor/HookPainter/PointDeleter.h
        behavior/BehavioredEntity.h
        shipEditor/EntityPlacer/EntityRegistry/EntityPlacement.cpp
        shipEditor/EntityPlacer/EntityRegistry/EntityPlacement.h
        entities/scripts/ShortLivedEntity.h
        loadGame/GameState.cpp
        loadGame/GameState.h
        userInterface/elements/entities/GUITooltip.cpp
        userInterface/elements/entities/GUITooltip.h
        player/VehicleLeave.cpp
        player/VehicleLeave.h
        player/PlayerVehicleTracker.cpp
        player/PlayerVehicleTracker.h
        entities/scripts/LateUpdateEntity.cpp
        entities/scripts/LateUpdateEntity.h
        physics/shapes/RoundStaticPhysicsShape.cpp
        physics/shapes/RoundStaticPhysicsShape.h
        physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorStaticRound.h
        entities/scripts/ActiveWhenPausedEntity.cpp
        entities/scripts/ActiveWhenPausedEntity.h
        game/PauseManager.cpp
        game/PauseManager.h
        userInterface/elements/GUI/GUIColoredRect.cpp
        userInterface/elements/GUI/GUIColoredRect.h
        loadGame/AutoSave.cpp
        loadGame/AutoSave.h
        game/ElementContainer.h
        userInterface/elements/GUI/GUIFramedLabel.h)

target_compile_definitions(SpaceshipsLib PRIVATE
        ENV_PROJECT_ROOT="${CMAKE_SOURCE_DIR}/"
)

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