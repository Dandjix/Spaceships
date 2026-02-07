#include <SDL3/SDL.h>
#include <utility>

#include "ElementContainer.h"
#include "PauseManager.h"
#include "debug/CollisionInfo.h"
#include "debug/PointCastChecker.h"
#include "entityRendering/RenderingInitialization.h"
#include "gameEvent/GameEvent.h"
#include "gameEvent/GetMousePositionType.h"
#include "loadGame/AutoSave.h"
#include "loadGame/GameState.h"
#include "loadGame/LoadSavedGame.h"
#include "player/PlayerVehicleTracker.h"
#include "player/InteractableInteract.h"
#include "player/VehicleLeave.h"
#include "spaceships/exterior/SpaceShipExterior.h"
#include "spaceships/exterior/exteriors/TestExterior.h"
#include "textures/UsageMap.h"
#include "userInterface/elements/GUI/GUILabel.h"
#include "userInterface/elements/notification/Snackbar.h"
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
#include "game/ElementContainer.h"
#include "../parallax/ParallaxObject.h"

void renderSpaceBackground() {
}

void renderPlanets() {
}

void renderParallax(SDL_Renderer *renderer, const ExteriorRenderingContext &context, SpaceShip *ship,
                    std::vector<ParallaxObject> objects) {
    std::stable_sort(objects.begin(), objects.end());
    for (auto object: objects) {
        object.render(renderer, context, ship);
    }
}

void quickSave(const GameState::GameState &game_state, std::string *save_name = nullptr) {
    auto new_save_path = Saves::getNewAutosavePath();
    if (save_name != nullptr) {
        *save_name = new_save_path.filename();
    }
    dumpGameState(game_state, new_save_path);
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

void RenderingHandle(SDL_Renderer *renderer, Camera *camera, const Vector2Int &screenDimensions,
                     std::vector<ParallaxObject> parallax_objects) {
    renderSpaceBackground();
    renderPlanets();

    auto ship = camera->working_spaceship;


    RenderingContext interiorContext = {
        {
            camera->getPosition(), camera->getAngle(), screenDimensions, camera->getScale()
        }
    };

    ExteriorRenderingContext exteriorContext = {
        {
            camera->getPosition(), ship->getPosition(), ship->getCenterOffset(), camera->getAngle(), ship->getAngle(), screenDimensions,
            camera->getScale()
        }
    };

    renderParallax(renderer, exteriorContext, ship, std::move(parallax_objects));

    //the one place that hasn't been corrupted by capitalism (it is space)
    ship->renderExterior(renderer, exteriorContext);

    //current ship interior rendering
    ship->renderInterior(renderer, interiorContext);

    //render
    ship->renderEntities(renderer, interiorContext);
}


void guiUpdateHandling(ElementContainer<GUIRect *> &gui_elements,
                       ElementContainer<GUIRect *> &gui_elements_deletion_queue,
                       const GUI_UpdateContext &gui_update_context, bool paused) {
    for (auto gui_element: gui_elements.get()) {
        gui_element->update(gui_update_context);
    }
    gui_elements.removeAndDelete(gui_elements_deletion_queue.get());
    gui_elements_deletion_queue.clear();
}

enum GameNavigation {
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

    // Entity rendering setup ------------------------------------------------------------------------------------------
    auto texture_usage_map = Textures::UsageMap(ENV_PROJECT_ROOT"assets/textures",renderer);
    EntityRendering::Context entity_loading_context = {texture_usage_map};

    // Texture Setup ---------------------------------------------------------------------------------------------------
    Tiles::loadAll(renderer);
    SpaceShipResources::TestExterior::loadAssets(renderer);

    // Setup -----------------------------------------------------------------------------------------------------------
    Camera *camera;
    BehavioredEntity *player;
    SpaceShip *player_spaceship;
    std::vector<SpaceShip *> space_ships;
    {
        GameState::GameState game_state = GameState::loadGameState(path_to_save, EntityId::Manager::getInstance(), &entity_loading_context);
        camera = game_state.getCamera();
        player = game_state.getPlayer();
        player_spaceship = game_state.getPlayerSpaceship();
        space_ships = game_state.space_ships;
    }
    camera->setPlayer(player);
    player_spaceship->setPlayer(player);

    // GUI elements ----------------------------------------------------------------------------------------------------
    ElementContainer<GUIRect *> gui_elements;
    ElementContainer<GUIRect *> gui_elements_deletion_queue;
    auto tooltip = new GUITooltip({0, 0}, false);
    gui_elements.add(tooltip);

    auto snackbar = new GUI::Snackbar(&gui_elements, &gui_elements_deletion_queue, Anchor::BottomCenter, {0, 0},
                                      GUI_Fill, 55, QueueOrder::FIRST);
    gui_elements.add(snackbar);

    // Short lived entities --------------------------------------------------------------------------------------------
    Player::PlayerVehicleTracker * vehicle_tracker = (new Player::PlayerVehicleTracker(player))->initializeRendering(entity_loading_context);
    //Debug stuff
    Debug::CollisionInfo * collision_info = (new Debug::CollisionInfo())->initializeRendering(entity_loading_context);

    auto *vehicle_enter = new Player::InteractableInteract(tooltip, vehicle_tracker);
    auto *vehicle_leave = new Player::VehicleLeave(vehicle_tracker);
    auto *pause_manager = new PauseManager(&paused);
    auto *pause_menu = new PauseMenu(pause_manager, {
                                         {"Resume Game", [pause_manager]() { pause_manager->setPaused(false); }},
                                         {
                                             "Quick Save", [space_ships,&snackbar]() {
                                                 std::string save_name;
                                                 quickSave(GameState::GameState(space_ships,EntityId::Manager::getInstance().getNextEntityId()), &save_name);
                                                 snackbar->addMessage(
                                                     std::format("Successfully saved game to : {}", save_name), 2000);

                                                 unsigned long deleted_count = Saves::deleteOldAutosaves();
                                                 if (deleted_count > 0) {
                                                     snackbar->addMessage(
                                                         std::format("Cleanup : deleted {} autosaves", deleted_count),
                                                         2000);
                                                 }
                                             }
                                         },
                                         {
                                             "Save and Quit to Main Menu",
                                             [&destination]() { destination = SaveAndMainMenu; }
                                         },
                                         {
                                             "Save and Quit to Desktop",
                                             [&destination]() { destination = SaveAndDesktop; }
                                         },
                                         {
                                             "Quit without saving to Main Menu",
                                             [&destination]() { destination = NoSaveAndMainMenu; }
                                         },
                                         {
                                             "Quit without saving to Desktop",
                                             [&destination]() { destination = NoSaveAndDesktop; }
                                         },
                                     }, &gui_elements);
    pause_manager->on_paused_change.subscribe([](bool paused) {
        // std::cout << "paused set to : " << paused << std::endl;
    });

    player_spaceship->registerEntities({vehicle_tracker, vehicle_enter, vehicle_leave, pause_manager, collision_info});


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
            GameEvent::getMousePositionType(gui_elements.get(), {mouse_x, mouse_y}),
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
            for (auto gui_element: gui_elements.get()) {
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
        guiUpdateHandling(gui_elements, gui_elements_deletion_queue, gui_update_context, paused);

        // RENDERING ---------------------------------------------------------------------------------------------------
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        RenderingHandle(renderer, camera, screenDimensions, parallax_objects);

        // GUI RENDERING -----------------------------------------------------------------------------------------------
        GUI_RenderingContext gui_rendering_context = {
            {
                camera->getPosition(),
                camera->getAngle(),
                screenDimensions,
                camera->getScale()
            }
        };

        gui_elements.sort([](GUIRect *first, GUIRect *second) {
            return first->getQueueOrder() > second->getQueueOrder();
        });
        for (auto gui_element: gui_elements.get()) {
            gui_element->render(renderer, gui_rendering_context);
        }

        SDL_RenderPresent(renderer);
    }

    MenuNavigation::Navigation navigation;

    if (destination == SaveAndDesktop || destination == SaveAndMainMenu) {
        auto game_state = GameState::GameState(
            space_ships,
            EntityId::Manager::getInstance().getNextEntityId()
        );
        GameState::dumpGameState(game_state, Saves::getNewAutosavePath());
        Saves::deleteOldAutosaves();
    }

    if (destination == SaveAndMainMenu || destination == NoSaveAndMainMenu) {
        navigation = MenuNavigation::MainMenu;
    } else {
        navigation = MenuNavigation::Quit;
    }

    delete pause_menu;
    for (auto space_ship: space_ships) {
        delete space_ship->finalizeRendering(entity_loading_context);
    }

    return navigation;
}
