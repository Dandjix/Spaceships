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

const int gridSize = 64;

void ResizeGrid(Vector2Int newSize)
{
    std::cout << "resizing to " << newSize.x << newSize.y << std::endl;
}

template <typename T>
void deleteItems(std::vector<T>* active,
                 const std::vector<T>& to_remove)
{
    std::unordered_set<T> remove_set(to_remove.begin(), to_remove.end());


    active->erase(
        std::remove_if(active->begin(), active->end(),
            [&](const T& item) {
                return  remove_set.contains(item);
            }),
        active->end()
    );

    for (auto e: to_remove) {
        delete e;
    }
}

MenuNavigation RunShipEditor(SDL_Renderer * renderer, SDL_Window * window)
{
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
                                            // INITIAL SETUP
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    Tiles::loadAll(renderer);

    std::vector<Entity *> activeEntities = {};
    std::vector<Entity *> activeEntitiesDeletionQueue = {};

    // std::vector<Entity*> inactiveEntities = {};

    std::vector<GUIRect*> editorGUIElements = {};
    std::vector<GUIRect*> editorGUIElementsDeletionQueue = {};



    auto camera = new FreeCamera(Vector2Int(0, 0), 0, 1,600);
    activeEntities.push_back(camera);

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    Vector2Int initialDimensions = Vector2Int(16, 16);

    float deltaTime = 0.0f;

    MenuNavigation destination = ShipEditor;

	//SpaceShipBlueprint blueprint = SpaceShipBlueprint::load("assets/spaceships/corvette.json");
	std::vector<std::vector<Tile>> blueprintTiles(initialDimensions.x, std::vector<Tile>(initialDimensions.y, Tile::Void));
	SpaceShipBlueprint blueprint = SpaceShipBlueprint("Untitled", "", blueprintTiles,{{},{}});

    ShipBuildingGrid grid(
        initialDimensions,
		camera,
        [&blueprint](Vector2Int newDimensions)
        {
            //std::cout << "new dimensions : " << newDimensions.x << " : " << newDimensions.y << std::endl;
            blueprint.resize(newDimensions);
        }
    );
    activeEntities.push_back(&grid);

    BlueprintEditorAppearance appearance(&blueprint);
    activeEntities.push_back(&appearance);

    ShipEditorModes::CommonEditorObjects common = {
        &blueprint,
        &grid,
        &appearance,
        &blueprintTiles
    };

    ShipEditorModes::ShipEditorStateMachine state_machine = ShipEditorModes::ShipEditorStateMachine(
        &common,
        &activeEntities,
        &activeEntitiesDeletionQueue,
        &editorGUIElements,
        &editorGUIElementsDeletionQueue,
        ShipEditorModes::TilePainter,
        window);

    std::vector<std::string>actionOptions =
    {
        "Resize",
        "Save",
        "Load",
        "Exit",
        "Edit tiles",
        "Edit hooks"
    };
    GUIList actionsList(
        Anchor::TL,
        Vector2Int(0, 0),
        100,
        GUI_Fill,
        actionOptions,
        [&destination,&grid,&blueprint,&state_machine](const std::string& option) {
            if (option == "Resize")
            {
                grid.startResizing();
            }
            else if (option == "Save")
            {
                SaveShip(blueprint.dumps());
            }
            else if (option == "Load")
            {
                std::string name;
                std::string content = LoadShip(&name);
                blueprint = SpaceShipBlueprint::loads(content, name);
                Vector2Int dimensions = Vector2Int(blueprint.tiles.size(), blueprint.tiles[0].size());
                grid.setDimensions(dimensions);

            }
            else if (option == "Exit")
            {
                destination = MainMenu;
            }
            else if (option =="Edit tiles") {
                state_machine.setMode(ShipEditorModes::TilePainter);
            }
            else if (option == "Edit hooks") {
                state_machine.setMode(ShipEditorModes::HookPainter);
            }
        }
       );
    editorGUIElements.push_back(&actionsList);


    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
                                                // EDITOR LOOP
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    while (destination == ShipEditor) {
        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);

        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        auto mouse_position_type = GameEvent::getMousePositionType(editorGUIElements, {mouse_x,mouse_y});

        GameEvent::GameEventContext event_context = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            },
            mouse_position_type,
            window
        };

        //handling events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = Quit;
            }
            for (Entity * entity : activeEntities)
            {
                entity->handleEvent(event, event_context);
            }
            for (GUIRect * element : editorGUIElements)
            {
                element->handleEvent(event, event_context);
            }
        }

        camera->setScreenDimensions(screenDimensions);

        //creating update context
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds

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
        for (Entity * entity : activeEntities)
        {
            entity->update(updateContext);
        }


        GUI_UpdateContext gui_updateContext = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale(),
            },
            deltaTime
        };

        //GUI update
        for (GUIRect * element : editorGUIElements)
        {
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

        //render
        for (Entity * entity : activeEntities)
        {
            entity->render(renderer,renderingContext);
        }

        //render debug
        for (Entity * entity : activeEntities)
        {
            entity->debugRender(renderer,renderingContext);
        }

        GUI_RenderingContext GUI_renderingContext(screenDimensions);

        //GUI render
        for (GUIRect * element : editorGUIElements)
        {
            element->render(renderer,GUI_renderingContext);
        }

        SDL_RenderPresent(renderer);

        deleteItems(&activeEntities,activeEntitiesDeletionQueue);
        activeEntitiesDeletionQueue.clear();

        deleteItems(&editorGUIElements,editorGUIElementsDeletionQueue);
        editorGUIElementsDeletionQueue.clear();
    }

    return destination;
}