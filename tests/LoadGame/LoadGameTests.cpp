//
// Created by timon on 12/3/25.
//

#include <fstream>

#include "gtest/gtest.h"
#include "loadGame/AutoSave.h"

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
TEST(AutosaveTests,NewAutoSaveTestWithoutExisting) {
    auto save_path = Saves::getNewAutosavePath("/home/timon/Projects/Spaceships/tests/LoadGame/saves_sandbox_1");
    ASSERT_EQ(save_path.filename(),"autosave_1.save.json");
}

TEST(AutosaveTests,NewAutoSaveTestWithExisting) {
    auto save_path = Saves::getNewAutosavePath("/home/timon/Projects/Spaceships/tests/LoadGame/saves_sandbox_2");
    ASSERT_EQ(save_path.filename(),"autosave_2.save.json");
}

TEST(AutosaveTests,DeletionTest) {
    auto saves_path = "/home/timon/Projects/Spaceships/tests/LoadGame/saves_sandbox_3";

    for (const auto& entry : std::filesystem::directory_iterator(saves_path))
        std::filesystem::remove_all(entry.path());

    for (int i = 0; i < 30; ++i) {
        auto path = Saves::getNewAutosavePath(saves_path);
        std::ofstream outfile (path);
        outfile << "this is an empty file for testing purposes" << std::endl;
        outfile.close();
    }

    ASSERT_EQ(Saves::deleteOldAutosaves(saves_path),14);
}