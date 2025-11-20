//
// Created by timon on 11/20/25.
//

#include <gtest/gtest.h>

#include "math/Vectors.h"
TEST(VectorTestSuite, LengthTest)
{
    Vector2Int vec = {
        100000000,
        100000000
    };
    // getting the length requires squaring those big numbers, which would overflow if they are not converted to float

    auto l = vec.length();

    auto ls = vec.sqrLength();

    SUCCEED();
}