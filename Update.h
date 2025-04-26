#pragma once
#include "Vectors.h"

struct UpdateContext {
	float deltaTime;
	Vector2Int screenDimensions;
	Vector2Int mouseWorldPosition;
};