//
// Created by timon on 2/6/26.
//
#include "gtest/gtest.h"
#include "math/Vectors.h"
#include "physics/ConvexSegmentCast.h"
#include "physics/Physics.h"


TEST(SegmentCastTestSuite, SegmentIntersectionTest) {
    ASSERT_EQ(Physics::segmentIntersection({0,0},{100,0},{50,50},{50,-50}), Vector2Int(50,0));

    ASSERT_EQ(Physics::segmentIntersection({-100,-100},{100,100},{-100,50},{100,-50}), Vector2Int(0,0));

    ASSERT_EQ(Physics::segmentIntersection({0,0},{0,100},{200,50},{300,-50}), std::nullopt);
}

TEST(SegmentCastTestSuite, SegmentIntersectionFloatsTest) {
    auto res_1 = Physics::segmentIntersectionFloats({0, 0}, {200, 0}, {50, 150}, {50, -50});
    ASSERT_TRUE(res_1.success);
    ASSERT_EQ(res_1.t1,0.25f);
    ASSERT_EQ(res_1.t2,0.75f);

    auto res_2 = Physics::segmentIntersectionFloats({-100, -100}, {100, 100}, {-100, 50}, {100, -50});
    ASSERT_TRUE(res_2.success);
    ASSERT_EQ(res_2.t1,0.5f);
    ASSERT_EQ(res_2.t2,0.5f);

    ASSERT_FALSE(Physics::segmentIntersectionFloats({0,0},{0,100},{200,50},{300,-50}).success);
}

TEST(SegmentCastTestSuite, SegmentCastTest) {
    std::vector<Vector2Int> points = {
        {-50, -50}, {50, -50},
        {50, 50}, {-50, 50}
    };

    ASSERT_EQ(Physics::localSegmentCast({0,0},{0,100},points), Vector2Int(0,50));

    ASSERT_EQ(Physics::localSegmentCast({0,0},{100,100},points), Vector2Int(50,50));

    ASSERT_EQ(Physics::localSegmentCast({0,0},{200,100},points), Vector2Int(50,25));

    // a point inside the points would need to be checked with is_inside()
    ASSERT_EQ(Physics::localSegmentCast({0,0},{25,25},points), std::nullopt);
}
