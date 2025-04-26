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

	GUI_RenderingContext(Vector2Int screenDimensions) : 
		screenDimensions(screenDimensions){}
};