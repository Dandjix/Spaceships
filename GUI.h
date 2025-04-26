#pragma once
#include "Vectors.h"
enum class Anchor
{
	TL,
	TR,
	BR,
	BL,
	CenterX,
	CenterY,
	Center
};

enum Fill
{
	GUI_Fill = -1
};

struct GUI_RenderingContext {
	const Vector2Int screenDimensions;
};

struct GUI_UpdateContext {
	const Vector2Int screenDimensions;
	const float deltaTime;
};