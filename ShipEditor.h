#pragma once
#include "MenuNavigation.h"
#include "SaveAndLoadShips.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

enum class SidebarButtons {
    Void,
    Wall,
    HDoor,
    VdDoor,
    Floor,
    Resize,
    Save,
    Load,
    COUNT
};

MenuNavigation RunShipEditor(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);