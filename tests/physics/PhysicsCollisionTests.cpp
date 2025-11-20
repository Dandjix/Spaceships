//
// Created by timon on 11/20/25.
//

#include "gtest/gtest.h"
#include "../../src/spaceships/SpaceShip.h"

TEST(PhysicsCollisionTestSuite, RoundRoundSimpleTest)
{
    SpaceShip * space_ship = new SpaceShip();

    std::cout << "empty sc : " << space_ship << std::endl;
    ASSERT_TRUE(false);
}