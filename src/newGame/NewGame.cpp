//
// Created by timon on 11/29/25.
//

#include "NewGame.h"

#include "entities/Sphere.h"
#include "../entities/toggleableActivators/Button.h"
// #include "entities/toggleables/Lamp.h"
#include "spaceships/Hooks/HookPoint.h"
#include "vehicles/combatOutfit/CombatOutfit.h"
#include "vehicles/spaceship/PilotSeat.h"

std::filesystem::path NewGame::ConstructNewGame() {
    GameState::transientGameState transient_game_state = {};
    SpaceShipBlueprint * blueprint = SpaceShipBlueprint::load(ENV_PROJECT_ROOT"assets/spaceships/battleship.json",transient_game_state, true);

    auto player = new Humanoid(Vector2Int{128,128}.scaleToWorldPosition(),45,new PlayerBehavior());
    auto camera =  new Camera(Vector2Int(0,0),0,1);
    camera->setPlayer(player);

    // auto lamp = new Lamp(Vector2Int{300,256}.scaleToWorldPosition(),0,EntityId::Manager::getInstance().createEntityId(),true);

    std::vector<Entity * > entities = {
        player,
        camera,
        // lamp,
        // new Humanoid(Vector2Int{96,128}.scaleToWorldPosition(),45,nullptr),
        new Sphere(Vector2Int{256,128}.scaleToWorldPosition(),Scaling::scaleToWorld(45.0f)),
        new CargoContainer(Vector2Int{96,96}.scaleToWorldPosition(),22),
        new CombatOutfit(Vector2Int{196,128}.scaleToWorldPosition(),35),
        new CombatOutfit(Vector2Int{256,128}.scaleToWorldPosition(),35),
        new PilotSeat(blueprint->hooks.getPoint("pilot_seat")->position,0),
        // new Button(Vector2Int{256,256}.scaleToWorldPosition(),0,lamp)
    };

    auto * space_ship = new SpaceShip(blueprint,entities,{0,0},0);

    GameState::GameState game_state = GameState::GameState{
        {space_ship}
    };

    std::filesystem::path path = ENV_PROJECT_ROOT"assets/newGame/newGame.save.json";

    GameState::dumpGameState(game_state,path);

    for (auto ship: game_state.space_ships) {
        delete ship;
    }

    return path;
}
