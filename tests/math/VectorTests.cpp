//
// Created by timon on 11/20/25.
//

#include <gtest/gtest.h>

#include "math/Vectors.h"
TEST(VectorTestSuite, LengthTest) {
    Vector2Int vec = {
        100000000,
        100000000
    };
    // getting the length requires squaring those big numbers, which would overflow if they are not converted to float

    auto l = vec.length();

    auto ls = vec.sqrLength();

    SUCCEED();
}

TEST(VectorTestSuite, AngleTest) {
    struct vector_test {
        Vector2Int v1;
        Vector2Int v2;
        float expected;
    };
    std::vector<vector_test> tests = {
        {
            {0, 1},
            {1, 0},
            90
        },
        {
            {0, 1},
            {-1, 0},
            90
        },
        {
            {0, 1},
            {1000000, 0},
            90
        },
        {
            {0, 1},
            {1000000, 0},
            90
        }
    };
    for (auto test: tests) {
        ASSERT_EQ(test.v1.angle_to(test.v2), test.expected);
    }
    ASSERT_TRUE(std::isnan(Vector2Int(0,0).angle_to(Vector2Int(0,0))));

    //floats
    ASSERT_EQ(Vector2Float(1,0).angle_to(Vector2Float(1,1)),45);
}


TEST(VectorTestSuite, RotateCardinalTests) {

    Vector2Int test = {69,420};

    ASSERT_EQ(test.rotate(0),test.rotateCardinal(0));
    ASSERT_EQ(test.rotate(90),test.rotateCardinal(90));
    ASSERT_EQ(test.rotate(180),test.rotateCardinal(180));
    ASSERT_EQ(test.rotate(270),test.rotateCardinal(270));

}