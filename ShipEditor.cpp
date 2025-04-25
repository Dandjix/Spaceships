#include "MenuNavigation.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
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


MenuNavigation RunShipEditor(SDL_Renderer * renderer, SDL_Window * window, TTF_Font* font)
{


    FreeCamera camera(Vector2Int(0, 0), 0, 1,600);

    ShipBuildingGrid grid(64);
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    float deltaTime = 0.0f;

    MenuNavigation destination = ShipEditor;

    GUIList list(Anchor::TL, Vector2Int(0, 0), 200, GUI_Fill);

    SpaceShipBlueprint blueprint = SpaceShipBlueprint::load("assets/spaceships/corvette.json");

    while (destination == ShipEditor) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = Quit;
            }
            camera.handleEvent(event);
        }
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds


        // update
        grid.update(deltaTime);
        camera.update(deltaTime);

        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);
        Vector2Int cameraPos = camera.getOffsetPosition(screenDimensions);

        RenderingContext renderingContext(cameraPos, camera.getAngle(), screenDimensions, camera.getScale());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //render
        grid.render(renderer, renderingContext);
        camera.render(renderer, renderingContext);

        //render debug
        grid.debugRender(renderer, renderingContext);

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        GUI_RenderingContext GUI_renderingContext(screenDimensions, Vector2Int(static_cast<int>(mouseX), static_cast<int>(mouseY)));

        //GUI render
        list.render(renderer, GUI_renderingContext);

        SDL_RenderPresent(renderer);

    }

    return destination;
}