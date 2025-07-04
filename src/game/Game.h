#pragma once
#include <SDL3/SDL.h>
#include "../entities/CargoContainer.h"
#include "../entities/Sphere.h"
#include "../player/Camera.h"
#include "../debug/DebugGrid.h"
#include "../math/Vectors.h"
#include "../userInterface/MenuNavigation.h"

MenuNavigation RunGame(SDL_Renderer* renderer, SDL_Window* window);