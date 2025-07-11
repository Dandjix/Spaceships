// main_menu.h

#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "MenuNavigation.h"
#include "../clay/renderers/clay_renderer_SDL3.h"

MenuNavigation RunMainMenu(Clay_SDL3RendererData* rendererData, SDL_Window* window);
