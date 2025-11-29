#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <utility>

#include "gameEvent/GameEvent.h"
#include "LoadGame/LoadSavedGame.h"
#include "physics/PhysicsUpdateVisitor/PhysicsUpdateVisitorWall.h"

#ifndef ENV_PROJECT_ROOT
#define ENV_PROJECT_ROOT ""
#endif

#include "../userInterface/MenuNavigation.h"

#include "../player/Camera.h"
#include "../behavior/PlayerBehavior.h"

#include "../entities/CargoContainer.h"
#include "../entities/Sphere.h"
#include "../debug/DebugGrid.h"
#include "../entities/Humanoid.h"
#include "../spaceships/SpaceShipBlueprint.h"
#include "../spaceships/SpaceShip.h"
#include "../physics/RayCaster.h"

#include "../parallax/ParallaxObject.h"

void renderSpaceBackground()
{

}

void renderPlanets()
{

}

void renderParallax(SDL_Renderer * renderer, const RenderingContext & context,std::vector<ParallaxObject> objects)
{
    std::stable_sort(objects.begin(),objects.end());
    for (auto object : objects)
    {
        object.render(renderer,context);
    }
}

std::vector<ParallaxObject> generateParallaxObjects(SDL_Renderer * renderer,Vector2Int base_origin)
{
    auto roid_texture = IMG_LoadTexture(renderer,ENV_PROJECT_ROOT"assets/environment/parallax/roid.png");

    auto object_1 = ParallaxObject(base_origin,0,0,roid_texture,5);

    Vector2Int second_pos = base_origin + Vector2Int(256,256) * static_cast<float>(Vectors::getFactor());
    auto object_2 = ParallaxObject(second_pos,90,512*Vectors::getFactor(),roid_texture);

    Vector2Int third_pos = base_origin + Vector2Int(512,512) * static_cast<float>(Vectors::getFactor());
    auto object_3 = ParallaxObject(third_pos,90,10000*Vectors::getFactor(),roid_texture,100);

    std::vector<ParallaxObject> objects = {
        object_1,
        object_2,
        object_3,
    };

    return objects;
}

void RenderingHandle(SDL_Renderer* renderer, SpaceShip* ship, std::vector<ParallaxObject> parallax_objects, const RenderingContext &renderingContext)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    renderSpaceBackground();
    renderPlanets();
    renderParallax(renderer,renderingContext,std::move(parallax_objects));

    //the one place that hasn't been corrupted by capitalism (it is space)
    ship->renderExterior(renderer,renderingContext);

    //current ship interior rendering
    ship->renderInterior(renderer,renderingContext);

    //render
    ship->renderEntities(renderer, renderingContext);

    SDL_RenderPresent(renderer);
}



MenuNavigation::Navigation RunGame(SDL_Renderer * renderer, SDL_Window * window,std::filesystem::path path_to_save, float target_delta_time)  // target_delta_time = 1.0/60.0
{
    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    Tiles::loadAll(renderer);

    LoadSavedGame::SavedGameData save = LoadSavedGame::SavedGameData(path_to_save);

    auto space_ships = save.load_spaceships();


    std::vector<ParallaxObject> parallax_objects = generateParallaxObjects(renderer,{0,0});

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    float deltaTime = 0.0f;
    MenuNavigation::Navigation destination = MenuNavigation::Game;

    // while (destination == MenuNavigation::Game) {
    //     last = now;
    //     now = SDL_GetTicks();
    //     deltaTime = (now - last) / 1000.0f; // Convert ms to seconds
    //
    //     int screenWidth, screenHeight;
    //     SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    //     Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);
    //
    //     if (deltaTime < target_delta_time)
    //     {
    //         int ms_to_wait = static_cast<int>((target_delta_time - deltaTime) * 1000);
    //         deltaTime = target_delta_time;
    //         SDL_Delay(ms_to_wait);
    //     }
    //
    //     // if (deltaTime != target_delta_time)
    //     //     std::cout << "fps : " << std::fixed << std::setprecision(6) << deltaTime << "(" << 1.0f/deltaTime << " fps)" << "\n";
    //     GameEvent::GameEventContext event_context =
    //     {
    //         {
    //             camera->getPosition(),
    //             camera->getAngle(),
    //             screenDimensions,
    //             camera->getScale()
    //         },
    //         GameEvent::MousePositionType::Game,
    //         window
    //     };
    //
    //     SDL_Event event;
    //     while (SDL_PollEvent(&event)) {
    //         if (event.type == SDL_EVENT_QUIT) {
    //             destination = MenuNavigation::Quit;
    //         }
    //         ship->eventHandling(event,event_context);
    //     }
    //     // UPDATE ------------------------------------------------------------------------------------------------------
    //
    //     camera->setScreenDimensions(screenDimensions);
    //
    //     ship->updateHandling(
    //         {camera->getPosition(),camera->getAngle(),screenDimensions,camera->getScale()},
    //         deltaTime,
    //         GameEvent::Game);
    //     // QUEUE DELETION ----------------------------------------------------------------------------------------------
    //     ship->handleQueueDeletion();
    //     // PHYSICS -----------------------------------------------------------------------------------------------------
    //
    //     ship->physicsHandling(target_delta_time);
    //
    //     // RENDERING ---------------------------------------------------------------------------------------------------
    //
    //     RenderingContext renderingContext = {
    //         {
    //             camera->getPosition(), camera->getAngle(), screenDimensions, camera->getScale()
    //         }
    //     };
    //     RenderingHandle(renderer, ship, parallax_objects, renderingContext);
    // }
    return destination;
}
