#include <SDL3/SDL.h>
#include <utility>

#include "PauseManager.h"
#include "gameEvent/GameEvent.h"
#include "gameEvent/GetMousePositionType.h"
#include "loadGame/AutoSave.h"
#include "loadGame/GameState.h"
#include "loadGame/LoadSavedGame.h"
#include "player/PlayerVehicleTracker.h"
#include "player/VehicleEnter.h"
#include "player/VehicleLeave.h"
#include "userInterface/elements/GUI/GUILabel.h"
#include "userInterface/pauseMenu/PauseMenu.h"

#ifndef ENV_PROJECT_ROOT
#define ENV_PROJECT_ROOT ""
#endif

#include "../userInterface/MenuNavigation.h"

#include "../player/Camera.h"

#include "../entities/CargoContainer.h"
#include "../entities/Sphere.h"
#include "../debug/DebugGrid.h"
#include "../spaceships/SpaceShip.h"

#include "../parallax/ParallaxObject.h"

void renderSpaceBackground() {
}

void renderPlanets() {
}

void renderParallax(SDL_Renderer *renderer, const RenderingContext &context, std::vector<ParallaxObject> objects) {
    std::stable_sort(objects.begin(), objects.end());
    for (auto object: objects) {
        object.render(renderer, context);
    }
}

void quickSave(const GameState::GameState &game_state) {
    dumpGameState(game_state, Saves::getNewAutosavePath());
}

std::vector<ParallaxObject> generateParallaxObjects(SDL_Renderer *renderer, Vector2Int base_origin) {
    auto roid_texture = IMG_LoadTexture(renderer,ENV_PROJECT_ROOT"assets/environment/parallax/roid.png");

    auto object_1 = ParallaxObject(base_origin, 0, 0, roid_texture, 5);

    Vector2Int second_pos = base_origin + Vector2Int(256, 256) * static_cast<float>(Vectors::getFactor());
    auto object_2 = ParallaxObject(second_pos, 90, 512 * Vectors::getFactor(), roid_texture);

    Vector2Int third_pos = base_origin + Vector2Int(512, 512) * static_cast<float>(Vectors::getFactor());
    auto object_3 = ParallaxObject(third_pos, 90, 10000 * Vectors::getFactor(), roid_texture, 100);

    std::vector<ParallaxObject> objects = {
        object_1,
        object_2,
        object_3,
    };

    return objects;
}

void RenderingHandle(SDL_Renderer *renderer, SpaceShip *ship, std::vector<ParallaxObject> parallax_objects,
                     const RenderingContext &renderingContext) {
    renderSpaceBackground();
    renderPlanets();
    renderParallax(renderer, renderingContext, std::move(parallax_objects));

    //the one place that hasn't been corrupted by capitalism (it is space)
    ship->renderExterior(renderer, renderingContext);

    //current ship interior rendering
    ship->renderInterior(renderer, renderingContext);

    //render
    ship->renderEntities(renderer, renderingContext);
}


void guiUpdateHandling(std::vector<GUIRect *> gui_elements, GUI_UpdateContext gui_update_context, bool paused) {
    for (auto gui_element: gui_elements) {
        gui_element->update(gui_update_context);
    }
}

enum GameNavigation{
    Game,
    SaveAndDesktop,
    SaveAndMainMenu,
    NoSaveAndDesktop,
    NoSaveAndMainMenu
};

MenuNavigation::Navigation RunGame(SDL_Renderer *renderer, SDL_Window *window,
                                   const std::filesystem::path &path_to_save,
                                   float target_delta_time) // target_delta_time = 1.0/60.0
{
    // Game loop setup -------------------------------------------------------------------------------------------------
    bool paused = false;
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    float deltaTime = 0.0f;
    GameNavigation destination = Game;
    // -----------------------------------------------------------------------------------------------------------------

    CargoContainer::LoadTextures(renderer);
    Sphere::LoadTextures(renderer);
    Tiles::loadAll(renderer);

    // Setup -----------------------------------------------------------------------------------------------------------
    Camera *camera;
    BehavioredEntity *player;
    SpaceShip *player_spaceship;
    std::vector<SpaceShip *> space_ships;
    {
        GameState::GameState game_state = GameState::loadGameState(path_to_save);
        camera = game_state.getCamera();
        player = game_state.getPlayer();
        player_spaceship = game_state.getPlayerSpaceship();
        space_ships = game_state.space_ships;
    }
    camera->setPlayer(player);
    player_spaceship->setPlayer(player);

    // GUI elements ----------------------------------------------------------------------------------------------------
    auto tooltip = new GUITooltip({0, 0}, false);
    // auto *label = new GUILabel(Anchor::Center,{0,0},100,50,"test",{255,255,255,255},fonts["lg"]);
    std::vector<GUIRect *> gui_elements = {tooltip};

    // Short lived entities --------------------------------------------------------------------------------------------
    auto *vehicle_tracker = new Player::PlayerVehicleTracker(player);
    auto *vehicle_enter = new Player::VehicleEnter(tooltip, vehicle_tracker);
    auto *vehicle_leave = new Player::VehicleLeave(vehicle_tracker);
    auto *pause_manager = new PauseManager(&paused);
    auto *pause_menu = new PauseMenu(pause_manager,{
        {"Resume Game",[pause_manager](){ pause_manager->setPaused(false);}},
        {"Quick Save",[space_ships](){quickSave(GameState::GameState(space_ships));}},
        {"Save and Quit to Main Menu",[&destination](){destination = SaveAndMainMenu;}},
        {"Save and Quit to Desktop",[&destination](){destination = SaveAndDesktop;}},
        {"Quit without saving to Main Menu",[&destination](){destination = NoSaveAndMainMenu;}},
        {"Quit without saving to Desktop",[&destination](){destination = NoSaveAndDesktop;}},
    },&gui_elements);
    pause_manager->on_paused_change.subscribe([](bool paused) {
        // std::cout << "paused set to : " << paused << std::endl;
    });

    player_spaceship->registerEntities({vehicle_tracker, vehicle_enter, vehicle_leave, pause_manager});


    // Parallax --------------------------------------------------------------------------------------------------------
    std::vector<ParallaxObject> parallax_objects = generateParallaxObjects(renderer, {0, 0});

    // GUI Testing -----------------------------------------------------------------------------------------------------
    // gui_elements.push_back(new GUILabel(Anchor::TL,{0,0},100,25,"TL",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::TR,{0,0},100,25,"TR",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::BL,{0,0},100,25,"BL",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::BR,{0,0},100,25,"BR",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::Center,{0,0},100,25,"Center",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::CenterLeft,{0,0},100,25,"CenterLeft",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::CenterRight,{0,0},100,25,"CenterRight",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::TopCenter,{0,0},100,25,"TopCenter",{255,255,255,255},fonts["sm"]));
    // gui_elements.push_back(new GUILabel(Anchor::BottomCenter,{0,0},100,25,"BottomCenter",{255,255,255,255},fonts["sm"]));


    while (destination == Game) {
        last = now;
        now = SDL_GetTicks();
        deltaTime = static_cast<float>(now - last) / 1000.0f; // Convert ms to seconds

        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        Vector2Int screenDimensions = Vector2Int(screenWidth, screenHeight);

        if (deltaTime < target_delta_time) {
            int ms_to_wait = static_cast<int>((target_delta_time - deltaTime) * 1000);
            deltaTime = target_delta_time;
            SDL_Delay(ms_to_wait);
        }

        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        GameEvent::GameEventContext event_context =
        {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale(),
            },
            GameEvent::getMousePositionType(gui_elements, {mouse_x, mouse_y}),
            window,
        };

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            for (auto space_ship: space_ships) {
                if (event.type == SDL_EVENT_QUIT) {
                    destination = NoSaveAndDesktop;
                }
                space_ship->eventHandling(event, event_context, paused);
            }
            for (auto gui_element: gui_elements) {
                gui_element->handleEvent(event, event_context);
            }
        }
        // UPDATE ------------------------------------------------------------------------------------------------------

        camera->setScreenDimensions(screenDimensions);


        for (auto ship: space_ships) {
            ship->updateHandling(
                {camera->getPosition(), camera->getAngle(), screenDimensions, camera->getScale()},
                deltaTime,
                GameEvent::Game,
                paused);
            // QUEUE DELETION ------------------------------------------------------------------------------------------
            ship->handleQueueDeletion();
            // PHYSICS -------------------------------------------------------------------------------------------------
            ship->physicsHandling(target_delta_time);

            ship->lateUpdateHandling(
                {camera->getPosition(), camera->getAngle(), screenDimensions, camera->getScale()},
                deltaTime,
                GameEvent::Game,
                paused);
        }

        GUI_UpdateContext gui_update_context = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            },
            deltaTime
        };
        guiUpdateHandling(gui_elements, gui_update_context,paused);

        // RENDERING ---------------------------------------------------------------------------------------------------
        RenderingContext renderingContext = {
            {
                camera->getPosition(), camera->getAngle(), screenDimensions, camera->getScale()
            }
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        RenderingHandle(renderer, camera->working_spaceship, parallax_objects, renderingContext);

        // GUI RENDERING -----------------------------------------------------------------------------------------------
        GUI_RenderingContext gui_rendering_context = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            }
        };
        std::ranges::stable_sort(gui_elements,[](GUIRect * first, GUIRect * second) {
            return first->getQueueOrder() > second->getQueueOrder();
        });
        for (auto gui_element: gui_elements) {
            gui_element->render(renderer, gui_rendering_context);
        }

        SDL_RenderPresent(renderer);
    }

    MenuNavigation::Navigation navigation;

    if (destination == SaveAndDesktop || destination == SaveAndMainMenu) {
        auto game_state = GameState::GameState(
            space_ships
        );
        GameState::dumpGameState(game_state,Saves::getNewAutosavePath());
    }

    if (destination == SaveAndMainMenu || destination == NoSaveAndMainMenu) {
        navigation = MenuNavigation::MainMenu;
    }
    else {
        navigation = MenuNavigation::Quit;
    }

    delete pause_menu;
    for (auto space_ship: space_ships) {
        delete space_ship;
    }

    return navigation;
}
