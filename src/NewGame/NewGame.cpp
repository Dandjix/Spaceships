//
// Created by timon on 11/29/25.
//

#include "NewGame.h"

std::filesystem::path NewGame::ConstructNewGame() {

    SpaceShipBlueprint * blueprint = SpaceShipBlueprint::load(ENV_PROJECT_ROOT"assets/spaceships/battleship.json");

    auto player = new Humanoid(Vector2Int{128,128}.scaleToWorldPosition(),45,new PlayerBehavior());
    auto camera =  new Camera(Vector2Int(0,0),0,1);
    camera->setPlayer(player);

    std::vector<Entity * > entities = {
        player,
        camera,
        new Humanoid(Vector2Int{96,128}.scaleToWorldPosition(),45,nullptr),
        new CargoContainer(Vector2Int{96,96}.scaleToWorldPosition(),22)
    };

    auto * space_ship = new SpaceShip(blueprint,entities);

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
