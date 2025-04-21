#pragma once
#include <SDL3/SDL.h>
#include "CargoContainer.h"
#include "Sphere.h"
#include "Camera.h"
#include "DebugGrid.h"
#include "Vectors.h"
#include "MenuNavigation.h"

MenuNavigation RunGame(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);