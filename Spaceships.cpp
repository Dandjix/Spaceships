#include <SDL3/SDL.h>
#include <iostream>
#include "Camera.h"
#include "Player.h"

#include "CargoContainer.h"
#include "Sphere.h"
#include "DebugGrid.h"



int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Spaceships", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;

    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    
    std::cout << "textures loaded";

    Player player(Vector2Int(0,0),0, 2000); // Start at center, 2000 units/sec
    Camera camera(Vector2Int(0,0),0,1, &player);
    CargoContainer container1(Vector2Int(0,0),45,CargoContainer::Variation::EMA);
    CargoContainer container2(Vector2Int(100, 0), 90, CargoContainer::Variation::SN);
    Sphere sphere(Vector2Int(-100,-100),32);
    DebugGrid grid(0,0,16);
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    float deltaTime = 0.0f;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            camera.handleEvent(event);
        }
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds


        // update
        player.update(deltaTime);
        container1.update(deltaTime);
        container2.update(deltaTime);
        grid.update(deltaTime);
        camera.update(deltaTime);
        sphere.update(deltaTime);

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
        player.render(renderer, renderingContext);
        container1.render(renderer, renderingContext);
        container2.render(renderer, renderingContext);
        sphere.render(renderer, renderingContext);

        //render debug
        container1.debugRender(renderer, renderingContext);
        container2.debugRender(renderer, renderingContext);
        sphere.debugRender(renderer, renderingContext);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}