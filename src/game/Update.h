#pragma once
#include "../math/Vectors.h"

class SpaceShip;

struct UpdateContext {
	float deltaTime;
	Vector2Int screenDimensions;
	SpaceShip* spaceShip;
};