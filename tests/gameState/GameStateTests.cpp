//
// Created by timon on 1/25/26.
//

#include "gtest/gtest.h"

#include "entities/Humanoid.h"
#include "entities/toggleables/Lamp.h"
#include "loadGame/GameState.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/SpaceShipBlueprint.h"

TEST(GameStateTestSuite, EntityDuplication_EmptyBlueprint_Test) {
    GameState::transientGameState transient_game_state = {};

    auto id_manager = EntityId::Manager();

    auto blueprint = SpaceShipBlueprint::load("/home/timon/Projects/Spaceships/tests/gameState/empty_test_ship.json",
                                              transient_game_state, id_manager, nullptr, true);


    GameState::GameState game_state = GameState::GameState{
        {
            new SpaceShip(blueprint, {
                              new Lamp({64, 64}, 45, id_manager.createEntityId(), false)
                          }, {0, 0}, 0)
        },
        0
    };

    auto dumped = GameState::dumpsGameState(game_state);
    nlohmann::json json = nlohmann::json::parse(dumped);
    ASSERT_EQ(json["spaceships"][0]["instance"].size(), 1);

    auto loaded = GameState::loadsGameState(dumped, id_manager, nullptr);

    ASSERT_EQ(loaded.space_ships.at(0)->instance->entities.size(), 1);
}

TEST(GameStateTestSuite, EntityDuplication_BlueprintWithLamp_Test) {
    GameState::transientGameState transient_game_state = {};
    auto id_manager = EntityId::Manager();

    auto blueprint = SpaceShipBlueprint::load("/home/timon/Projects/Spaceships/tests/gameState/test_ship_1_lamp.json",
                                              transient_game_state, id_manager, nullptr, true);

    GameState::GameState game_state = GameState::GameState{
        {
            new SpaceShip(blueprint, {}, {0, 0}, 0)
        },
        0
    };

    auto dumped = GameState::dumpsGameState(game_state);
    nlohmann::json json = nlohmann::json::parse(dumped);
    ASSERT_EQ(json["spaceships"][0]["instance"].size(), 1);

    auto loaded = GameState::loadsGameState(dumped, id_manager, nullptr);

    ASSERT_EQ(loaded.space_ships.at(0)->instance->entities.size(), 1);
}

TEST(GameStateTestSuite, EntityDuplication_BlueprintWithLampPlusEntities_Test) {
    GameState::transientGameState transient_game_state = {};
    auto id_manager = EntityId::Manager();

    auto blueprint = SpaceShipBlueprint::load("/home/timon/Projects/Spaceships/tests/gameState/test_ship_1_lamp.json",
                                              transient_game_state, id_manager, nullptr, true);


    EntityId::entityId id = id_manager.createEntityId();

    GameState::GameState game_state = GameState::GameState{
        {
            new SpaceShip(blueprint, {
                              new Lamp({64, 64}, 45, id, false)
                          }, {0, 0}, 0)
        },
        0
    };

    auto dumped = GameState::dumpsGameState(game_state);
    nlohmann::json json = nlohmann::json::parse(dumped);
    ASSERT_EQ(json["spaceships"][0]["instance"].size(), 2);

    auto loaded = GameState::loadsGameState(dumped, id_manager, nullptr);
    ASSERT_EQ(loaded.space_ships.at(0)->instance->entities.size(), 2);
}

TEST(GameStateTestSuite, EntityDuplication_BlueprintWithHumanoid_Test) {
    GameState::transientGameState transient_game_state = {};
    auto id_manager = EntityId::Manager();

    auto blueprint = SpaceShipBlueprint::load(
        "/home/timon/Projects/Spaceships/tests/gameState/test_ship_1_humanoid.json",
        transient_game_state, id_manager, nullptr, true);

    ASSERT_EQ(blueprint->entities.size(), 1);

    GameState::GameState game_state = GameState::GameState{
        {
            new SpaceShip(blueprint, {}, {0, 0}, 0)
        },
        0
    };

    auto dumped = GameState::dumpsGameState(game_state);

    nlohmann::json json = nlohmann::json::parse(dumped);
    std::cout << json.dump(2) << std::endl;
    ASSERT_EQ(json["spaceships"].size(), 1);
    ASSERT_EQ(json["spaceships"][0]["instance"].size(), 1);

    auto loaded = GameState::loadsGameState(dumped, id_manager, nullptr);
    ASSERT_EQ(loaded.space_ships.size(), 1);
    ASSERT_EQ(loaded.space_ships.at(0)->instance->entities.size(), 1);
}
