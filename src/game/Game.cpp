#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

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

void PhysicsHandling(SpaceShip* ship, float target_delta_time, int subdivisions = 2)
{
    const PhysicsUpdateContext physicsContext = {
        target_delta_time / static_cast<float>(subdivisions),
        ship
    };

    for (int i = 0; i < subdivisions; ++i)
    {
        auto physics_entities = ship->getPhysicsEntities(RoomDistance::Close);

        for (int i = 0; i < physics_entities.size(); i++)
        {
            for (int j = i+1; j < physics_entities.size(); j++)
            {
                auto e1 = physics_entities.at(i);
                auto e2 = physics_entities.at(j);

                if (!e1->shape->getBoundingBox().intersects(e2->shape->getBoundingBox()))
                {
                    continue;
                }

                PhysicsUpdateVisitor *  visitor = e1->shape->createVisitor();

                e2->shape->consumeVisitor(visitor,ship);

                delete visitor;
            }
        }
    }

    for (PhysicsEntity * e : ship->getPhysicsEntities(RoomDistance::All))
    {
        PhysicsUpdateVisitorWall visitor = PhysicsUpdateVisitorWall();
        e->shape->consumeVisitor(&visitor,ship);
    }


    for (PhysicsEntity * entity : ship->getPhysicsEntities(RoomDistance::All))
    {
        entity->shape->physicsUpdate(physicsContext);
    }
}

void renderEntities(SDL_Renderer* renderer, SpaceShip* ship, RenderingContext renderingContext)
{
    for (Entity* entity : ship->getEntities(RoomDistance::Immediate))
    {
        entity->render(renderer, renderingContext);
    }
    //render debug
    for (Entity* entity : ship->getEntities(RoomDistance::Immediate))
    {
        entity->debugRender(renderer, renderingContext);
    }
    //physics shapes debug
    for (PhysicsEntity * entity : ship->getPhysicsEntities(RoomDistance::Immediate))
    {
        entity->shape->debugRender(renderer, renderingContext);
    }
}

void UpdateHandling(SDL_Window* window, Camera* camera, SpaceShip* ship, float deltaTime, Vector2Int& screenDimensions)
{
    const UpdateContext updateContext =
    {
        deltaTime,
        Vector2Int(0,0),
        ship
    };


    camera->setScreenDimensions(screenDimensions);

    // update
    for (Entity * entity : ship->getEntities(RoomDistance::All))
    {
        entity->update(updateContext);
    }
    ship->update(updateContext);
}

void RenderingHandle(SDL_Renderer* renderer, SpaceShip* ship, std::vector<ParallaxObject> parallax_objects, RenderingContext renderingContext)
{
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
    renderEntities(renderer, ship, renderingContext);

    SDL_RenderPresent(renderer);
}

MenuNavigation RunGame(SDL_Renderer * renderer, SDL_Window * window, float target_delta_time)  // target_delta_time = 1.0/60.0
{
    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    Tiles::loadAll(renderer);

    std::cout << "textures loaded" << std::endl;

    // entities

    // Vector2Int base_origin = Vector2Int(1,1)*std::numeric_limits<int>::max()*0.9f;
    // Vector2Int base_origin = Vector2Int(6400,6400).scaleToWorldPosition();
    Vector2Int base_origin = {0,0};

    Camera * camera = new Camera(base_origin, 0, 1);

    PlayerBehavior * playerBehavior = new PlayerBehavior(camera);

    SDL_Texture * player_texture = IMG_LoadTexture(renderer,ENV_PROJECT_ROOT"assets/entities/player/player_placeholder.png");

    Humanoid * player = new Humanoid(base_origin+Vector2Int(200,200).scaleToWorldPosition(), 0, playerBehavior,player_texture); // Start at center, 200 pixels/sec
    camera->setPlayer(player);

    Humanoid * dummy = new Humanoid(base_origin + Vector2Int(300,300).scaleToWorldPosition(),0,nullptr,nullptr);

    // Humanoid * dummy2 = new Humanoid(base_origin + Vector2Int(300,300).scaleToWorldPosition(),0,nullptr,nullptr);
    //
    // Humanoid * dummy3 = new Humanoid(base_origin + Vector2Int(300,300).scaleToWorldPosition(),0,nullptr,nullptr);


    CargoContainer * container1 = new CargoContainer(base_origin + Vector2Int(500,500).scaleToWorldPosition(), 45, CargoContainer::Variation::EMA);
    CargoContainer * container2 = new CargoContainer(base_origin + Vector2Int(200, 300).scaleToWorldPosition(), 22, CargoContainer::Variation::SN);
    Sphere * sphere = new Sphere(Vector2Int(700, 200).scaleToWorldPosition(),  Scaling::scaleToWorld(50.0f),nullptr);
    // DebugGrid * grid = new DebugGrid(0, 0, 64);
    RayCaster* rayCaster = new RayCaster(camera, player);
    // Cursor* cursor = new Cursor(camera);

    SpaceShipBlueprint blueprint = SpaceShipBlueprint::load(ENV_PROJECT_ROOT"assets/spaceships/battleship.json");
    SpaceShip * ship = new SpaceShip(&blueprint);

    ship->registerEntities(
        { 
        camera,
        player,
        dummy,
        container1,
        container2,
        sphere,
        // grid,
        rayCaster,
        // cursor
        }
    );

    //parallax



    std::vector<ParallaxObject> parallax_objects = generateParallaxObjects(renderer,base_origin);

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    float deltaTime = 0.0f;
    MenuNavigation destination = Game;

    ship->setFocusEntity(player);

    while (destination == Game) {
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // Convert ms to seconds

        if (deltaTime < target_delta_time)
        {
            int ms_to_wait = static_cast<int>((target_delta_time - deltaTime) * 1000);
            deltaTime = target_delta_time;
            SDL_Delay(ms_to_wait);
        }

        // if (deltaTime != target_delta_time)
        //     std::cout << "fps : " << std::fixed << std::setprecision(6) << deltaTime << "(" << 1.0f/deltaTime << " fps)" << "\n";


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

        // PHYSICS -----------------------------------------------------------------------------------------------------

        PhysicsHandling(ship,target_delta_time);

        // UPDATE ------------------------------------------------------------------------------------------------------

        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);

        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);

        UpdateHandling(window, camera, ship, deltaTime, screenDimensions);

        Vector2Int cameraPos = camera->getPosition();

        RenderingContext renderingContext(cameraPos, camera->getAngle(), screenDimensions, camera->getScale());

        RenderingHandle(renderer, ship, parallax_objects, renderingContext);
    }
    return destination;
}
