//
// Created by timon on 1/26/26.
//
#include "entities/toggleableActivators/Button.h"
#include "gtest/gtest.h"
#include "loadGame/GameState.h"
#include "spaceships/SpaceShipBlueprint.h"

TEST(BlueprintLoadingTestSuite, BlueprintLoadingTest) {


    GameState::transientGameState transient_game_state = {};

    EntityId::Manager manager = EntityId::Manager();

    SpaceShipBlueprint * blueprint = SpaceShipBlueprint::load(
        "/home/timon/Projects/Spaceships/tests/gameState/blueprintLoading/working_example.json",
        transient_game_state,
        manager,
        true
        );

    ASSERT_EQ(dynamic_cast<Button *>(blueprint->entities[0])->getLinkedEntity() , blueprint->entities[1]);

    std::cout << "deleting !" << std::endl;
    delete blueprint;
}