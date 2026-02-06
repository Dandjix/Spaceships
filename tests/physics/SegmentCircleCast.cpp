//
// Created by timon on 2/6/26.
//

#include "physics/SegmentCircleCast.h"

#include "gtest/gtest.h"
#include "physics/Physics.h"
TEST(SegmentCircleCastTestSuite, SegmentCircleCastTest) {
    auto res_1 = Physics::segmentCircleIntersection({0, 0}, {100, 0}, {50, 0}, 25);
    ASSERT_TRUE(res_1.has_value());
    ASSERT_EQ(res_1.value(),Vector2Int(25,0));

    auto res_2 = Physics::segmentCircleIntersection({0, 0}, {100, 0}, {50, 25}, 50);
    ASSERT_TRUE(res_2.has_value());
    ASSERT_EQ(res_2.value(), Vector2Int(7, 0));

    auto res_3 = Physics::segmentCircleIntersection({0, 0}, {100, 0}, {55, 55}, 50);
    ASSERT_FALSE(res_3.has_value());
}

TEST(SegmentCircleCastTestSuite, SegmentCircleCastExtremeTest) {
    Vector2Int offset = {
        static_cast<int>(static_cast<float>(std::numeric_limits<int>::max()) * 0.9f),
        static_cast<int>(static_cast<float>(std::numeric_limits<int>::max()) * 0.9f)
    };

    auto res_1 = Physics::segmentCircleIntersection(offset + Vector2Int(0, 0), offset + Vector2Int(100, 0), offset + Vector2Int(50, 0), 25);
    ASSERT_TRUE(res_1.has_value());
    ASSERT_EQ(res_1.value(),offset + Vector2Int(25,0));

    auto res_2 = Physics::segmentCircleIntersection(offset + Vector2Int(0, 0), offset + Vector2Int(100, 0), offset + Vector2Int(50, 25), 50);
    ASSERT_TRUE(res_2.has_value());
    ASSERT_EQ(res_2.value(),offset + Vector2Int(7, 0));

    auto res_3 = Physics::segmentCircleIntersection(offset + Vector2Int(0, 0), offset + Vector2Int(100, 0), offset + Vector2Int(55, 55), 50);
    ASSERT_FALSE(res_3.has_value());
}

TEST(SegmentCircleCastTestSuite, SegmentCircleCastFloatTest) {
    auto res_1 = Physics::segmentCircleIntersectionFloat({0, 0}, {100, 0}, {50, 0}, 25);
    ASSERT_TRUE(res_1.has_value());
    ASSERT_NEAR(res_1.value(),0.25,0.01f);

    auto res_2 = Physics::segmentCircleIntersectionFloat({0, 0}, {100, 0}, {50, 25}, 50);
    ASSERT_TRUE(res_2.has_value());
    ASSERT_NEAR(res_2.value(), 0.07,0.01f);

    auto res_3 = Physics::segmentCircleIntersectionFloat({0, 0}, {100, 0}, {55, 55}, 50);
    ASSERT_FALSE(res_3.has_value());

    auto res_4 = Physics::segmentCircleIntersectionFloat({0, 0}, {100, 0}, {150, 0}, 45);
    ASSERT_FALSE(res_4.has_value());

    auto res_5 = Physics::segmentCircleIntersectionFloat({0, 0}, {100, 0}, {-50, 0}, 45);
    ASSERT_FALSE(res_5.has_value());
}