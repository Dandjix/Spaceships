#pragma once
#include <SDL3/SDL.h>
#include "../entities/CargoContainer.h"
#include "../entities/Sphere.h"
#include "../player/Camera.h"
#include "../debug/DebugGrid.h"
#include "../math/Vectors.h"
#include "../userInterface/MenuNavigation.h"
#include "../clay/renderers/clay_renderer_SDL3.h"

MenuNavigation RunGame(const Clay_SDL3RendererData * rendererData, SDL_Window* window);