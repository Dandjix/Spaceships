#pragma once
#include "../math/Vectors.h"
#include "gameEvent/GameEvent.h"

class SpaceShip;

struct UpdateContext {
	float deltaTime;
	Vector2Int screenDimensions;
	SpaceShip* spaceShip;
	GameEvent::MousePositionType mouse_position_type;
};