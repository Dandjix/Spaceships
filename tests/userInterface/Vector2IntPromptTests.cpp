//
// Created by timon on 2/28/26.
//

#include "gtest/gtest.h"
#include "userInterface/elements/prompts/Vector2IntPrompt.h"

TEST(Vector2IntPromptTestSuite, SimpleVector2IntPromptTest) {

    GUI::Prompts::Vector2IntPrompt prompt = GUI::Prompts::Vector2IntPrompt(Anchor::Center,{0,0},100,100,nullptr);

    prompt.setValue({100,50});
    ASSERT_TRUE(prompt.inputIsValid());
    ASSERT_EQ(prompt.getValue(),Vector2Int(100,50));

    prompt.setRawValue("200,150");
    ASSERT_TRUE(prompt.inputIsValid());
    ASSERT_EQ(prompt.getValue(),Vector2Int(200,150));

    prompt.setRawValue("200");
    ASSERT_FALSE(prompt.inputIsValid());

    prompt.setRawValue("100,100,100");
    ASSERT_FALSE(prompt.inputIsValid());

    prompt.setRawValue("[100,101]");
    ASSERT_TRUE(prompt.inputIsValid());
    ASSERT_EQ(prompt.getValue(),Vector2Int(100,101));

    prompt.setRawValue("{100,101}");
    ASSERT_TRUE(prompt.inputIsValid());
    ASSERT_EQ(prompt.getValue(),Vector2Int(100,101));

    prompt.setRawValue("(100,101)");
    ASSERT_TRUE(prompt.inputIsValid());
    ASSERT_EQ(prompt.getValue(),Vector2Int(100,101));
}