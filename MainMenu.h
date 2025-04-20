// main_menu.h

#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "MenuNavigation.h"

MenuNavigation RunMainMenu(SDL_Renderer* renderer, TTF_Font* font);
