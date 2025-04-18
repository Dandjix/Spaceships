#pragma once
#include "Vectors.h"

struct RenderingContext
{
	const Vector2Int cameraPos;
	const float cameraAngle;

	const Vector2Int screenDimensions;

	const float cameraScale;

	RenderingContext(Vector2Int cameraPos, float cameraAngle, Vector2Int screenDimensions, float cameraScale)
		: cameraPos(cameraPos), cameraAngle(cameraAngle), screenDimensions(screenDimensions),cameraScale(cameraScale){}
};