#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "MenuNavigation.h"

#include "Camera.h"
#include "Player.h"

#include "Cursor.h"

#include "CargoContainer.h"
#include "Sphere.h"
#include "DebugGrid.h"
#include "SpaceShipBlueprint.h"
#include "SpaceShip.h"
#include "RayCaster.h"


MenuNavigation RunGame(SDL_Renderer * renderer, SDL_Window * window)
{
    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    Tiles::loadAll(renderer);

    std::cout << "textures loaded";

    Camera * camera = new Camera(Vector2Int(0, 0), 0, 1);
    Player * player = new Player(Vector2Int(64*5, 64*5), 0, 2000, camera); // Start at center, 200 pixels/sec
    camera->setPlayer(player);

    CargoContainer * container1 = new CargoContainer(Vector2Int(0, 0), 45, CargoContainer::Variation::EMA);
    CargoContainer * container2 = new CargoContainer(Vector2Int(100, 0), 90, CargoContainer::Variation::SN);
    Sphere * sphere = new Sphere(Vector2Int(-5, -5), 32);
    DebugGrid * grid = new DebugGrid(0, 0, 64);
    RayCaster* rayCaster = new RayCaster(camera, player);
    Cursor* cursor = new Cursor(camera);

    SpaceShipBlueprint blueprint = SpaceShipBlueprint::load("assets/spaceships/battleship.json");
    SpaceShip * ship = new SpaceShip(&blueprint);

    ship->registerEntities(
        { 
        camera,
        player,
        container1,
        container2,
        sphere,
        //grid,
        rayCaster,
        cursor
        }
    );

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