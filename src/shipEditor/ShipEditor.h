#pragma once
#include "../userInterface/MenuNavigation.h"
#include "../spaceships/SaveAndLoadShips.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>


MenuNavigation RunShipEditor(SDL_Renderer* renderer, SDL_Window* window);