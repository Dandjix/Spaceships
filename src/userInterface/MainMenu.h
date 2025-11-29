// main_menu.h

#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "MenuNavigation.h"

MenuNavigation::Navigation RunMainMenu(SDL_Renderer *renderer, SDL_Window *window);
