#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "userInterface/MainMenu.h"
#include "game/Game.h"
#include "shipEditor/ShipEditor.h"
#include "userInterface/fonts.h"
#include "userInterface/elements/GUI/GUIList.h"

void main_menu()
{

}

int main(int argc, char* argv[]) {

    if (!TTF_Init()) {
        std::cout << "Failed to initialize SDL_ttf" << std::endl;
        // handle error
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!LoadFonts())
    {
        std::cout << "could not load fonts" << std::endl;
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
            break;
        case MainMenu:
            navigation = RunMainMenu(renderer, window);
            break;
        case ShipEditor:
            navigation = RunShipEditor(renderer, window);
            break;
        case Settings:
            throw std::logic_error("Function not yet implemented");
            break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}