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

    std::cout << ENV_PROJECT_ROOT << std::endl;

    SpaceShipBlueprint * blueprint = SpaceShipBlueprint::load(
        "/home/timon/Projects/Spaceships/tests/gameState/blueprintLoading/working_example.json",
        transient_game_state,
        manager,
        nullptr,
        true,
        true
        );

    auto button =dynamic_cast<Button *>(blueprint->entities.at(0));

    auto toggleable = button->getLinkedEntity();

    ASSERT_TRUE(toggleable != nullptr);

    auto linked = toggleable->asEntity();

    auto lamp = blueprint->entities.at(1);

    ASSERT_EQ(linked, lamp);

    std::cout << "deleting !" << std::endl;
    delete blueprint;
}