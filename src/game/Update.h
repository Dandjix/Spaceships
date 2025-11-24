#pragma once
#include "../math/Vectors.h"
#include "gameEvent/GameEvent.h"

class SpaceShip;

struct UpdateContext {
	CameraTransformations::CameraInfo camera_info;

	float deltaTime;
	SpaceShip* spaceShip;
	GameEvent::MousePositionType mouse_position_type;
};