#include <SDL3/SDL.h>
#include <iostream>
#include "Player.h"
#include "Camera.h"
#include "CargoContainer.h"
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

    std::cout << "textures loaded";

    Player player(320, 240, 200); // Start at center, 200 pixels/sec
    Camera camera(&player);
    CargoContainer container1(0,0,45,CargoContainer::Variation::EMA);
    CargoContainer container2(100, 0, 270, CargoContainer::Variation::SL);
    CargoContainer container3(0, 100, 0, CargoContainer::Variation::SN);
    CargoContainer container4(100, 100, 12, CargoContainer::Variation::blank);
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
        }
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds


        // update
        player.update(deltaTime);

        container1.update(deltaTime);
        container2.update(deltaTime);
        container3.update(deltaTime);
        container4.update(deltaTime);

        grid.update(deltaTime);
        camera.update(deltaTime);
        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Float cameraPos = toVector2Float(camera.getOffsetPosition(screenWidth,screenHeight));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        //render
        camera.render(renderer, cameraPos, screenWidth, screenHeight);
        player.render(renderer, cameraPos, screenWidth, screenHeight);
        
        container1.render(renderer, cameraPos, screenWidth, screenHeight);
        container2.render(renderer, cameraPos, screenWidth, screenHeight);
        container3.render(renderer, cameraPos, screenWidth, screenHeight);
        container4.render(renderer, cameraPos, screenWidth, screenHeight);
        grid.render(renderer, cameraPos, screenWidth, screenHeight);



        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}