#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <functional>
#include <vector>
#include <string>

#include "../userInterface/MenuNavigation.h"
#include "../math/Vectors.h"
#include "../player/FreeCamera.h"
#include "../shipEditor/ShipEditor.h"
#include "../shipEditor/ShipBuildingGrid.h"
#include "../spaceships/SaveAndLoadShips.h"
#include "../spaceships/SpaceShipBlueprint.h"
#include "../shipEditor/BlueprintTilePainter.h"
#include "../userInterface/GUIList.h"
#include "../spaceships/Tile.h"
#include "../shipEditor/BlueprintEditorAppearance.h"
#include "userInterface/GUICheckbox.h"
#include "userInterface/GUILabel.h"

const int gridSize = 64;

void ResizeGrid(Vector2Int newSize)
{
    std::cout << "resizing to " << newSize.x << newSize.y << std::endl;
}

//inputs

//pr� physique
//d�cisions IA

//physique
//mouvements, d�tection de collision, gravit�

//post physique
//r�ponse des collisions, MAJ position camera

//render

MenuNavigation RunShipEditor(SDL_Renderer * renderer, SDL_Window * window)
{
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
                                            // INITIAL SETUP
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================

    std::vector<Entity*> activeEntities = {};

    // std::vector<Entity*> inactiveEntities = {};

    std::vector<GUIRect*> editorGUIElements = {};



    Tiles::loadAll(renderer);

    FreeCamera camera(Vector2Int(0, 0), 0, 1,600);
    activeEntities.push_back(&camera);

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    Vector2Int initialDimensions = Vector2Int(16, 16);

    float deltaTime = 0.0f;

    MenuNavigation destination = ShipEditor;

	//SpaceShipBlueprint blueprint = SpaceShipBlueprint::load("assets/spaceships/corvette.json");
	std::vector<std::vector<Tile>> blueprintTiles(initialDimensions.x, std::vector<Tile>(initialDimensions.y, Tile::Void));
	SpaceShipBlueprint blueprint = SpaceShipBlueprint("Untitled", "", blueprintTiles);

    ShipBuildingGrid grid(
        initialDimensions,
		&camera,
        [&blueprint](Vector2Int newDimensions)
        {
            //std::cout << "new dimensions : " << newDimensions.x << " : " << newDimensions.y << std::endl;
            blueprint.resize(newDimensions);
        }
    );
    activeEntities.push_back(&grid);

    BlueprintTilePainter painter = BlueprintTilePainter(&blueprint, &grid, Tile::Wall,false);
    activeEntities.push_back(&painter);

    BlueprintEditorAppearance appearance(&blueprint);
    activeEntities.push_back(&appearance);
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
                                            // GUI ELEMENTS
    // #================================================================================================================
    // #================================================================================================================
    // #================================================================================================================
    std::vector<std::string>tileOptions =
    {
        "Void",
        "Wall",
        "DoorH",
        "DoorV",
        "Floor"
    };
    GUIList tilesList(
        Anchor::TL,
        Vector2Int(0, 0),
        100,
        GUI_Fill,
        tileOptions,
        [&painter](const std::string& option) {
            painter.setTileToPaint(Tiles::tileFromName(option));
        },
        true
    );
    editorGUIElements.push_back(&tilesList);

    std::vector<std::string>actionOptions =
    {
        "Resize",
        "Save",
        "Load",
        "Exit"
    };
    GUIList actionsList(
        Anchor::TL,
        Vector2Int(100, 0),
        100,
        150,
        actionOptions,
        [&destination,&grid,&blueprint](const std::string& option) {
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
        }
       );
    editorGUIElements.push_back(&actionsList);

    editorGUIElements.push_back(new GUILabel(Anchor::TL,{200,0},50,25,"Fill",{255,255,255},fonts["sm"]));

    GUICheckbox fillCheckbox(
        Anchor::TL,
        {250,0},
        [&painter](bool checkboxValue){
            painter.fill = checkboxValue;
        },
        painter.fill
    );
    editorGUIElements.push_back(&fillCheckbox);

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

        //handling events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = Quit;
            }
            for (Entity * entity : activeEntities)
            {
                entity->handleEvent(event);
            }
            for (GUIRect * element : editorGUIElements)
            {
                element->handleEvent(event);
            }
        }

        camera.setScreenDimensions(screenDimensions);

        //creating update context
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds

        UpdateContext updateContext = { 
            deltaTime, 
            screenDimensions,
        };

        // update
        for (Entity * entity : activeEntities)
        {
            entity->update(updateContext);
        }


        GUI_UpdateContext gui_updateContext = {
            screenDimensions,
            deltaTime
        };

        //GUI update
        for (GUIRect * element : editorGUIElements)
        {
            element->update(gui_updateContext);
        }


        Vector2Int cameraPos = camera.getPosition();

        RenderingContext renderingContext = {
            cameraPos,
            camera.getAngle(),
            screenDimensions,
            camera.getScale()
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

    }

    return destination;
}