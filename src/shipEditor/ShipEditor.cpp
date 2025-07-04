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

const int gridSize = 64;

void ResizeGrid(Vector2Int newSize)
{
    std::cout << "resizing to " << newSize.x << newSize.y << "\n";
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
    Tiles::loadAll(renderer);

    FreeCamera camera(Vector2Int(0, 0), 0, 1,600);


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
        [&destination,&grid,&blueprint](std::string option) {
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

	BlueprintTilePainter painter = BlueprintTilePainter(&blueprint, &grid, Tile::Wall);

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
        [&painter](std::string option) {
            painter.setTileToPaint(Tiles::tileFromName(option));
        },
        true
    );

    BlueprintEditorAppearance appearance(&blueprint);

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
			painter.handleEvent(event);
			//important : grid must be registered before actions or resize will end instantly
            grid.handleEvent(event);
            camera.handleEvent(event);
            tilesList.handleEvent(event);
            actionsList.handleEvent(event);
            appearance.handleEvent(event);
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
        grid.update(updateContext);
        camera.update(updateContext);
        painter.update(updateContext);
        appearance.update(updateContext);


        GUI_UpdateContext gui_updateContext = {
            screenDimensions,
            deltaTime
        };

        //GUI update
        tilesList.update(gui_updateContext);
        actionsList.update(gui_updateContext);


        Vector2Int cameraPos = camera.getOffsetPosition(screenDimensions);

        RenderingContext renderingContext = {
            cameraPos,
            camera.getAngle(),
            screenDimensions,
            camera.getScale()
        };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //render
        camera.render(renderer, renderingContext);
        appearance.render(renderer, renderingContext);
        grid.render(renderer, renderingContext);
        painter.render(renderer, renderingContext);

        //render debug
        grid.debugRender(renderer, renderingContext);

        GUI_RenderingContext GUI_renderingContext(screenDimensions);

        //GUI render
        tilesList.render(renderer, GUI_renderingContext);
        actionsList.render(renderer, GUI_renderingContext);

        SDL_RenderPresent(renderer);

    }

    return destination;
}