//
// Created by timon on 1/26/26.
//

#include <fstream>

#include "json.hpp"
#include "entities/entityId/EntityId.h"
#include "gtest/gtest.h"

TEST(IncrementEntityIdTestSuite, IncrementEntityIdSimpleTest) {


    std::ifstream in("/home/timon/Projects/Spaceships/tests/gameState/incrementEntityId/test_ship_1_lamp_1_button.json");
    std::stringstream buffer;
    buffer << in.rdbuf();

    nlohmann::json json = nlohmann::json::parse(buffer);

    EntityId::Manager manager = EntityId::Manager();

    manager.setNextEntityId(1000);

    manager.incrementEntityIds(json);

    ASSERT_EQ(json["instance"][0]["entity_id"], 1000);
    ASSERT_EQ(json["instance"][1]["linked_entity_id"], 1000);

    ASSERT_EQ(manager.getNextEntityId(),1001);
}

TEST(IncrementEntityIdTestSuite, IncrementEntityIdDoubleTest) {


    std::ifstream in("/home/timon/Projects/Spaceships/tests/gameState/incrementEntityId/test_ship_2_lamp_2_button.json");
    std::stringstream buffer;
    buffer << in.rdbuf();

    nlohmann::json json = nlohmann::json::parse(buffer);

    EntityId::Manager manager = EntityId::Manager();

    manager.setNextEntityId(1000);

    manager.incrementEntityIds(json);

    ASSERT_EQ(json["instance"][0]["entity_id"], 1000);
    ASSERT_EQ(json["instance"][1]["entity_id"], 1001);
    ASSERT_EQ(json["instance"][2]["linked_entity_id"], 1000);
    ASSERT_EQ(json["instance"][3]["linked_entity_id"], 1001);

    ASSERT_EQ(manager.getNextEntityId(),1002);
}