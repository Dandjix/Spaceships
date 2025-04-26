#include "MenuNavigation.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <functional>
#include <vector>
#include <string>
#include "Vectors.h"
#include "FreeCamera.h"
#include "ShipEditor.h"
#include "ShipBuildingGrid.h"
#include "SaveAndLoadShips.h"
#include "SpaceShipBlueprint.h"
#include "GUIList.h"

const int gridSize = 64;

void ResizeGrid(Vector2Int newSize)
{
    std::cout << "resizing to " << newSize.x << newSize.y << "\n";
}

//inputs

//pré physique
//décisions IA

//physique
//mouvements, détection de collision, gravité

//post physique
//réponse des collisions, MAJ position camera

//render

MenuNavigation RunShipEditor(SDL_Renderer * renderer, SDL_Window * window)
{


    FreeCamera camera(Vector2Int(0, 0), 0, 1,600);

    ShipBuildingGrid grid(64);
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    float deltaTime = 0.0f;

    MenuNavigation destination = ShipEditor;

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
        [&destination,&grid](std::string option) {
            if (option == "Resize")
            {
                grid.startResizing();
            }
            else if (option == "Save")
            {

            }
            else if (option == "Load")
            {

            }
            else if (option == "Exit")
            {
                destination = MainMenu;
            }
        }
       );

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
        [](std::string option) {
            std::cout << "selected " << option << std::endl;
        },
        true
    );


    SpaceShipBlueprint blueprint = SpaceShipBlueprint::load("assets/spaceships/corvette.json");

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
            camera.handleEvent(event);
            tilesList.handleEvent(event);
            actionsList.handleEvent(event);
        }

        //creating update context
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds

        UpdateContext updateContext(deltaTime, screenDimensions);

        // update
        grid.update(updateContext);
        camera.update(updateContext);


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
        grid.render(renderer, renderingContext);
        camera.render(renderer, renderingContext);

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