#pragma once
#include "../userInterface/MenuNavigation.h"
#include "../spaceships/SaveAndLoadShips.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "../clay/renderers/clay_renderer_SDL3.h"

MenuNavigation RunShipEditor(Clay_SDL3RendererData* rendererData, SDL_Window* window);