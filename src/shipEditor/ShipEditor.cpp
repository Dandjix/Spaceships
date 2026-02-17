#include <SDL3/SDL.h>
#include <functional>
#include <vector>
#include <string>

#include "userInterface/MenuNavigation.h"
#include "math/Vectors.h"
#include "player/FreeCamera.h"
#include "shipEditor/ShipEditor.h"

#include "shipEditor/ShipBuildingGrid.h"
#include "spaceships/SaveAndLoadShips.h"
#include "spaceships/SpaceShipBlueprint.h"
#include "TilePainter/BlueprintTilePainter.h"
#include "../userInterface/elements/GUI/GUIList.h"
#include "spaceships/Tile.h"
#include "shipEditor/BlueprintEditorAppearance.h"
#include "gameEvent/GameEvent.h"
#include "gameEvent/GetMousePositionType.h"
#include "shipEditorModes/CommonEditorEntities.h"
#include "shipEditorModes/ShipEditorStateMachine.h"
#include "../userInterface/elements/GUI/GUICheckbox.h"
#include "entities/CargoContainer.h"
#include "entityRendering/RenderingInitialization.h"
#include "game/ElementContainer.h"
#include "loadGame/GameState.h"
#include "EntityPlacer/EntityPlacement/EntityPlacement.h"

void ResizeGrid(Vector2Int newSize) {
    std::cout << "resizing to " << newSize.x << newSize.y << std::endl;
}

#include <string>

int nthOccurrence(const std::string &str, const std::string &findMe, int nth) {
    size_t pos = 0;
    int cnt = 0;

    while (cnt != nth) {
        pos += 1;
        pos = str.find(findMe, pos);
        if (pos == std::string::npos)
            return -1;
        cnt++;
    }
    return pos;
}

MenuNavigation::Navigation RunShipEditor(SDL_Renderer *renderer, SDL_Window *window) {
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    // INITIAL SETUP
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    Tiles::loadAll(renderer);

    ElementContainer<Entity *> activeEntities = {};
    ElementContainer<Entity *> activeEntitiesDeletionQueue = {};

    // std::vector<Entity*> inactiveEntities = {};

    ElementContainer<GUIRect *> editorGUIElements = {};
    ElementContainer<GUIRect *> editorGUIElementsDeletionQueue = {};

    auto texture_usage_map = Textures::UsageMap(ENV_PROJECT_ROOT"assets/textures", renderer);
    EntityRendering::Context entity_rendering_context = {texture_usage_map};

    auto camera = new FreeCamera(Vector2Int(0, 0), 0, 1, 600);
    activeEntities.add(camera);

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    Vector2Int initialDimensions = Vector2Int(16, 16);

    float deltaTime = 0.0f;

    MenuNavigation::Navigation destination = MenuNavigation::ShipEditor;

    auto *blueprint = new SpaceShipBlueprint(
        "",
        "",
        std::vector<std::vector<Tile> >(initialDimensions.x, std::vector<Tile>(initialDimensions.y, Tile::Void)),
        {},
        new SpaceshipHooks({}, {}, {})
    );

    ShipBuildingGrid grid(
        initialDimensions,
        camera,
        [&blueprint](Vector2Int newDimensions) {
            //std::cout << "new dimensions : " << newDimensions.x << " : " << newDimensions.y << std::endl;
            blueprint->resize(newDimensions);
        }
    );
    activeEntities.add(&grid);

    BlueprintEditorAppearance appearance(blueprint);
    activeEntities.add(&appearance);

    auto *entity_placement_interface = new EntityPlacement::Interface();

    EntityPlacement::Context entity_placement_context = {entity_placement_interface};

    ShipEditorModes::CommonEditorObjects common = {
        blueprint,
        &grid,
        &appearance,
        &blueprint->tiles,
        &blueprint->entities,
        &entity_rendering_context,
        entity_placement_interface
    };

    ShipEditorModes::ShipEditorStateMachine state_machine = ShipEditorModes::ShipEditorStateMachine(
        &common,
        &activeEntities,
        &activeEntitiesDeletionQueue,
        &editorGUIElements,
        &editorGUIElementsDeletionQueue,
        ShipEditorModes::TilePainter,
        window);

    std::string blueprint_name = "untitled.json";

    GUIList actionsList(
        Anchor::TL,
        Vector2Int(0, 0),
        100,
        GUI_Fill,
        {
            "Resize",
            "Save",
            "Load",
            "Exit",
            "Edit tiles",
            "Edit hooks",
            "Edit airlocks",
            "Edit entities",
            "Link toggleables"
        },
        [&entity_rendering_context,&destination,&grid,&blueprint,&state_machine,&blueprint_name](
    const std::string &option) {
            if (option == "Resize") {
                grid.startResizing();
            } else if (option == "Save") {
                SaveShip(blueprint->dumps(), blueprint_name);
            } else if (option == "Load") {
                std::string path;
                std::string content = LoadShip(&path);

                if (content.empty()) {
                    return;
                }
                EntityId::Manager::getInstance().reset();
                GameState::transientGameState transient_game_state = {};

                auto loaded = SpaceShipBlueprint::loads(content, path, transient_game_state,
                                                        EntityId::Manager::getInstance(), &entity_rendering_context,
                                                        true, true);

                blueprint_name = path;
                *blueprint = *loaded;
                Vector2Int dimensions = Vector2Int(blueprint->tiles.size(), blueprint->tiles[0].size());
                grid.setDimensions(dimensions);
            } else if (option == "Exit") {
                destination = MenuNavigation::MainMenu;
            } else if (option == "Edit tiles") {
                state_machine.setMode(ShipEditorModes::TilePainter);
            } else if (option == "Edit hooks") {
                state_machine.setMode(ShipEditorModes::HookPainter);
            } else if (option == "Edit airlocks") {
                state_machine.setMode(ShipEditorModes::AirlockPainter);
            } else if (option == "Edit entities") {
                state_machine.setMode(ShipEditorModes::EntityPainter);
            } else if (option == "Link toggleables") {
                state_machine.setMode(ShipEditorModes::ToggleableLinker);
            }
        }
    );
    editorGUIElements.add(&actionsList);


    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    // EDITOR LOOP
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    while (destination == MenuNavigation::ShipEditor) {
        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);

        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        auto mouse_position_type = GameEvent::getMousePositionType(editorGUIElements.get(), {mouse_x, mouse_y});

        GameEvent::GameEventContext event_context = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            },
            nullptr,
            mouse_position_type,
            window
        };

        //handling events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = MenuNavigation::Quit;
            }
            for (Entity *entity: activeEntities.get()) {
                entity->handleEvent(event, event_context);
            }
            for (GUIRect *element: editorGUIElements.get()) {
                element->handleEvent(event, event_context);
            }
        }

        camera->setScreenDimensions(screenDimensions);

        //creating update context
        last = now;
        now = SDL_GetTicks();
        deltaTime = static_cast<float>(now - last) / 1000.0f; // Convert ms to seconds

        UpdateContext updateContext = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            },
            deltaTime,
            nullptr,
            mouse_position_type
        };

        // update
        for (Entity *entity: activeEntities.get()) {
            entity->update(updateContext);
        }


        GUI_UpdateContext gui_updateContext = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            },
            deltaTime
        };

        //GUI update
        for (GUIRect *element: editorGUIElements.get()) {
            element->update(gui_updateContext);
        }


        Vector2Int cameraPos = camera->getPosition();

        RenderingContext renderingContext = {
            cameraPos,
            camera->getAngle(),
            screenDimensions,
            camera->getScale()
        };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //Entities Rendering ------------------------------------------------------------------------------------
        std::vector<Entity *> all_entities = activeEntities.get();
        all_entities.insert(all_entities.begin(), blueprint->entities.begin(), blueprint->entities.end());

        std::ranges::stable_sort(all_entities, [](Entity *a, Entity *b) {
            return a->getQueueOrder() > b->getQueueOrder();
        });

        for (Entity *entity: all_entities) {
            // if (dynamic_cast<CargoContainer * >(entity) != nullptr) {
            //     std::cout << "rendering a container ! " << std::endl;
            // }

            entity->render(renderer, renderingContext);
        }
        for (Entity *entity: all_entities) {
            entity->debugRender(renderer, renderingContext);
        }

        GUI_RenderingContext GUI_renderingContext(
            {
                {0, 0},
                0,
                screenDimensions,
                1
            });

        //GUI render
        for (GUIRect *element: editorGUIElements.get()) {
            element->render(renderer, GUI_renderingContext);
        }

        SDL_RenderPresent(renderer);

        activeEntities.removeAndDelete(activeEntitiesDeletionQueue.get());
        activeEntitiesDeletionQueue.clear();

        editorGUIElements.removeAndDelete(editorGUIElementsDeletionQueue.get());
        editorGUIElementsDeletionQueue.clear();
    }

    delete blueprint;

    return destination;
}
