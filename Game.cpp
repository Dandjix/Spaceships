#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "MenuNavigation.h"

#include "Camera.h"
#include "Player.h"

#include "Cursor.h"

#include "CargoContainer.h"
#include "Sphere.h"
#include "DebugGrid.h"


MenuNavigation RunGame(SDL_Renderer * renderer, SDL_Window * window)
{
    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);

    std::cout << "textures loaded";

    Camera camera(Vector2Int(0, 0), 0, 1);
    Player player(Vector2Int(0, 0), 0, 2000, &camera); // Start at center, 200 px/sec
    camera.setPlayer(&player);

    CargoContainer container1(Vector2Int(0, 0), 45, CargoContainer::Variation::EMA);
    CargoContainer container2(Vector2Int(100, 0), 90, CargoContainer::Variation::SN);
    Sphere sphere(Vector2Int(-5, -5), 32);
    DebugGrid grid(0, 0, 64);
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    float deltaTime = 0.0f;

    MenuNavigation destination = Game;

    while (destination == Game) {
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

        const UpdateContext updateContext =
        {
            deltaTime,
            Vector2Int(0,0)
        };

        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);
        camera.setScreenDimensions(screenDimensions);

        // update
        camera.update(updateContext);
        player.update(updateContext);
        container1.update(updateContext);
        container2.update(updateContext);
        grid.update(updateContext);
        sphere.update(updateContext);


        Vector2Int cameraPos = camera.getOffsetPosition(screenDimensions);

        RenderingContext renderingContext(cameraPos, camera.getAngle(), screenDimensions, camera.getScale());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //render

        camera.render(renderer, renderingContext);
        grid.render(renderer, renderingContext);
        player.render(renderer, renderingContext);
        container1.render(renderer, renderingContext);
        container2.render(renderer, renderingContext);
        sphere.render(renderer, renderingContext);

        //render debug
        grid.debugRender(renderer, renderingContext);
        container1.debugRender(renderer, renderingContext);
        container2.debugRender(renderer, renderingContext);
        sphere.debugRender(renderer, renderingContext);
        SDL_RenderPresent(renderer);
    }
    return destination;
}