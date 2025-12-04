#pragma once
#include <utility>

#include "../math/Vectors.h"
#include "game/CameraTransformations.h"

enum class Anchor
{
	TL,
	TR,
	BR,
	BL,
	Center,
	BottomCenter,
	TopCenter,
	CenterLeft,
	CenterRight
};

enum Fill
{
	GUI_Fill = -1
};

struct GUI_RenderingContext
{
	CameraTransformations::CameraInfo camera_info;
};

struct GUI_UpdateContext
{
	CameraTransformations::CameraInfo camera_info;
	const float deltaTime;
};