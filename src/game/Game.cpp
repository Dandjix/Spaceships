#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

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
    for (auto object : objects)
    {
        object.render(renderer,context);
    }
}

std::vector<ParallaxObject> generateParallaxObjects(SDL_Renderer * renderer)
{
    auto roid_texture = IMG_LoadTexture(renderer,ENV_PROJECT_ROOT"assets/environment/parallax/roid.png");

    auto object_1 = ParallaxObject({0,0},0,0,roid_texture);

    Vector2Int second_pos = Vector2Int(256,256) * static_cast<float>(Vectors::getFactor());
    auto object_2 = ParallaxObject(second_pos,90,512*Vectors::getFactor(),roid_texture);

    Vector2Int third_pos = Vector2Int(512,512) * static_cast<float>(Vectors::getFactor());
    auto object_3 = ParallaxObject(third_pos,90,10000*Vectors::getFactor(),roid_texture,100);

    std::vector<ParallaxObject> objects = {
        object_1,
        object_2,
        object_3,
    };

    return objects;
}

MenuNavigation RunGame(SDL_Renderer * renderer, SDL_Window * window)
{
    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    Tiles::loadAll(renderer);

    std::cout << "textures loaded" << std::endl;

    // entities

    Camera * camera = new Camera(Vector2Int(0, 0), 0, 1);

    PlayerBehavior * playerBehavior = new PlayerBehavior(camera);


    Humanoid * player = new Humanoid(Vector2Int(0, 0), 0, playerBehavior); // Start at center, 200 pixels/sec
    camera->setPlayer(player);

    CargoContainer * container1 = new CargoContainer(Vector2Int(0, 0), 45, CargoContainer::Variation::EMA);
    CargoContainer * container2 = new CargoContainer(Vector2Int(100, 0), 90, CargoContainer::Variation::SN);
    Sphere * sphere = new Sphere(Vector2Int(-5, -5), 32);
    DebugGrid * grid = new DebugGrid(0, 0, 64);
    RayCaster* rayCaster = new RayCaster(camera, player);
    // Cursor* cursor = new Cursor(camera);

    SpaceShipBlueprint blueprint = SpaceShipBlueprint::load(ENV_PROJECT_ROOT"assets/spaceships/battleship.json");
    SpaceShip * ship = new SpaceShip(&blueprint);

    ship->registerEntities(
        { 
        camera,
        player,
        container1,
        container2,
        sphere,
        // grid,
        rayCaster,
        // cursor
        }
    );

    //parallax



    std::vector<ParallaxObject> parallax_objects = generateParallaxObjects(renderer);

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    float deltaTime = 0.0f;
    MenuNavigation destination = Game;

    ship->setFocusEntity(player);

    while (destination == Game) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destination = Quit;
            }
            for (Entity * entity : ship->getEntities(RoomDistance::All))
            {
                entity->handleEvent(event);
            }
        }
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds

        const UpdateContext updateContext =
        {
            deltaTime,
            Vector2Int(0,0),
            ship
        };

        //render variable calculation
        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);
        camera->setScreenDimensions(screenDimensions);

        // update
        for (Entity * entity : ship->getEntities(RoomDistance::All))
        {
            entity->update(updateContext);
        }
        ship->update(updateContext);

        Vector2Int cameraPos = camera->getOffsetPosition(screenDimensions);

        RenderingContext renderingContext(cameraPos, camera->getAngle(), screenDimensions, camera->getScale());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        renderSpaceBackground();
        renderPlanets();
        renderParallax(renderer,renderingContext,parallax_objects);

        //the one place that hasn't been corrupted by capitalism (it is space)
        ship->renderExterior(renderer,renderingContext);

        //current ship interior rendering
        ship->renderInterior(renderer,renderingContext);

        //render
        for (Entity* entity : ship->getEntities(RoomDistance::Immediate))
        {
            entity->render(renderer, renderingContext);
        }
        //render debug
		for (Entity* entity : ship->getEntities(RoomDistance::Immediate))
		{
			entity->debugRender(renderer, renderingContext);
		}
        
        SDL_RenderPresent(renderer);
    }
    return destination;
}
