#pragma once
#include "gameEvent/GameEvent.h"
class SpaceShip;

namespace Instances {
	class Instance;
}

struct UpdateContext {
	CameraTransformations::CameraInfo camera_info;

	float deltaTime;
	Instances::Instance * world_instance;
	GameEvent::MousePositionType mouse_position_type;
};