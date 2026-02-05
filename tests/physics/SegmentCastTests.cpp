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

TEST(SegmentCastTestSuite, SegmentCastTest) {
    std::vector<Vector2Int> points = {
        {-50, -50},{50,-50},
        {50,50},{-50,50}
    };

    ASSERT_EQ(Physics::localSegmentCast({0,0},{0,100},points),Vector2Int(0,50));

    ASSERT_EQ(Physics::localSegmentCast({0,0},{100,100},points),Vector2Int(50,50));

    ASSERT_EQ(Physics::localSegmentCast({0,0},{200,100},points),Vector2Int(50,25));

    // a point inside the points would need to be checked with is_inside()
    ASSERT_EQ(Physics::localSegmentCast({0,0},{25,25},points),std::nullopt);
}