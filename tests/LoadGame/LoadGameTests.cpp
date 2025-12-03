//
// Created by timon on 12/3/25.
//

#include "gtest/gtest.h"
#include "LoadGame/AutoSave.h"

TEST(AutosaveTests,SplitStringTest) {
    {
        auto split = Saves::splitString("autosave_1.save.json","_");
        ASSERT_EQ(split.size(),2);
        ASSERT_EQ(split[0],"autosave");
        ASSERT_EQ(split[1],"1.save.json");
    }
    {
        auto split = Saves::splitString("a_more_complete_example.save.json","_");

        ASSERT_EQ(split.size(),4);
        ASSERT_EQ(split[0],"a");
        ASSERT_EQ(split[1],"more");
        ASSERT_EQ(split[2],"complete");
        ASSERT_EQ(split[3],"example.save.json");
    }
}