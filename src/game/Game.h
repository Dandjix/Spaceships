#pragma once
#include <SDL3/SDL.h>
#include "../entities/CargoContainer.h"
#include "../entities/Sphere.h"
#include "../player/Camera.h"
// #include "../debug/DebugGrid.h"
#include "../math/Vectors.h"
#include "../userInterface/MenuNavigation.h"

MenuNavigation::Navigation RunGame(SDL_Renderer* renderer, SDL_Window* window,std::filesystem::path saved_game_path, float target_delta_time = 1.0f/120.0f);


