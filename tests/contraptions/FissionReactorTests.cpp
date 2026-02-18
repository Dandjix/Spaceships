//
// Created by timon on 2/18/26.
//

#include "entities/contraptions/fissionReactor/controlRod/ControlRod.h"
#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"
#include "gtest/gtest.h"

using Reactor = Contraptions::FissionReactor::Reactor;
using Rod = Contraptions::FissionReactor::ControlRod;

TEST(FissionReactorTestSuite, FissionReactorSerialTest) {
    std::vector<Reactor::ControlRodInfo> info;
    info = {
        {0, 0.5f},
        {45, 0.5f}
    };
    Reactor reactor = Reactor({0, 0}, 0, info, 1000);
    {
        auto back_to_info = reactor.generate_rod_info();

        ASSERT_EQ(back_to_info.at(0).relative_angle,0);
        ASSERT_EQ(back_to_info.at(1).relative_angle,45); //here 45 is the angle around the reactor, the angle of the entity is opposite
    }

    nlohmann::json json = reactor.toJson();
    GameState::transientGameState game_state = {};
    Reactor *back_to_reactor = Reactor::fromJson(json, game_state);
    {
        ASSERT_EQ(back_to_reactor->getControlRods().at(0)->getAngle(),0);
        ASSERT_EQ(back_to_reactor->getControlRods().at(1)->getAngle(),-45);
        //inverted since because it is PLACED at
        //45 deg clockwise of the reactor, which means
        //the angle of the actual entity is reversed
    }
    {
        auto back_to_info = back_to_reactor->generate_rod_info();

        ASSERT_EQ(back_to_info.at(0).relative_angle,0);
        ASSERT_EQ(back_to_info.at(1).relative_angle,45); //here 45 is the angle around the reactor
    }
}
