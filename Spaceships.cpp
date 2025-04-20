#include <SDL3/SDL.h>
#include <iostream>

#include "Game.h"
#include "MainMenu.h"

void main_menu()
{

}

int main(int argc, char* argv[]) {

    if (!TTF_Init()) {
        std::cout << "Failed to initialize SDL_ttf";
        // handle error
    }

    TTF_Font* font = TTF_OpenFont("assets/fonts/square-deal/square-deal.ttf", 24);  // 24 pt size

    if (!font) {
        std::cout << "Failed to load font";
        // handle error
    }

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

    MenuNavigation navigation = MainMenu;

    while (navigation != Quit)
    {
        switch (navigation)
        {
        case Game:
            navigation = RunGame(renderer, window);
        case MainMenu:
            navigation = RunMainMenu(renderer, font);
        case ShipEditor:
            break;
        case Settings:
            break;
        case Quit:
            break;
        default:
            break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}