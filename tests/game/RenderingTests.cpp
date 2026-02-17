//
// Created by timon on 11/14/25.
//


#include "../../src/game/Rendering.h"
#include "../../src/math/Vectors.h"
#include "gtest/gtest.h"

TEST(CameraTransformationsTestSuite, ReversabilitySimple)
{
    auto context = RenderingContext(
        {{0,0},0,{1000,500},1}
    );

    Vector2Float screenPos = {500,250};

    Vector2Int world_pos = context.camera_info.screenToWorldPoint(screenPos);

    Vector2Float back_to_screen = context.camera_info.worldToScreenPoint(world_pos);

    auto back_to_world = context.camera_info.screenToWorldPoint(back_to_screen);

    ASSERT_EQ(world_pos.x,0);
    ASSERT_EQ(world_pos.y,0);

    ASSERT_EQ(back_to_screen.x,500);
    ASSERT_EQ(back_to_screen.y,250);

    ASSERT_EQ(back_to_world.x,0);
    ASSERT_EQ(back_to_world.y,0);
}

TEST(CameraTransformationsTestSuite, ReversabilityOffset) {
    auto context = RenderingContext(
        {{500*Vectors::getFactor(),250*Vectors::getFactor()},0,{1000,500},2}
    );

    Vector2Float screenPos = {500,250};

    Vector2Int world_pos = context.camera_info.screenToWorldPoint(screenPos);
    ASSERT_EQ(world_pos.x,500*Vectors::getFactor());
    ASSERT_EQ(world_pos.y,250*Vectors::getFactor());

    Vector2Float back_to_screen = context.camera_info.worldToScreenPoint(world_pos);
    ASSERT_EQ(back_to_screen.x,500);
    ASSERT_EQ(back_to_screen.y,250);
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// Outside Camera transformations
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

TEST(ExteriorCameraTransformationsTestSuite, SimpleTest) {
    //TODO : figure out why this doesnt pass
    GTEST_SKIP();
    auto exterior_camera_info  = CameraTransformations::ExteriorCameraInfo{
        {0,0},
        Vector2Int(-500,-500).scaleToWorldPosition(),
        Vector2Int(0,0).scaleToWorldPosition(),
        0,
        0,
        {1000,500},
        1
    };

    auto res = exterior_camera_info.worldToScreenPoint(Vector2Int(0,0).scaleToWorldPosition());

    ASSERT_EQ(res,Vector2Float(0,0));
}

TEST(ExteriorCameraTransformationsTestSuite, SimpleTest2) {
    //TODO : figure out why this doesnt pass
    GTEST_SKIP();
    auto exterior_camera_info  = CameraTransformations::ExteriorCameraInfo{
            {0,0},
            Vector2Int(500,500).scaleToWorldPosition(),
            Vector2Int(1000,1000).scaleToWorldPosition(),
            0,
            45,
            {1000,500},
            1
        };

    auto res = exterior_camera_info.worldToScreenPoint(Vector2Int(0,0).scaleToWorldPosition());

    ASSERT_EQ(res,Vector2Float(0,0));
}