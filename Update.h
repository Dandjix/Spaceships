#pragma once
#include "Vectors.h"

struct UpdateContext {
	float deltaTime;
	Vector2Float mousePosition;
	Vector2Int screenDimensions;
};