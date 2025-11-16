#pragma once
#include "../math/Vectors.h"
#include "../spaceships/SpaceShip.h"
#include <cfloat>

namespace Physics
{
	std::optional<Vector2Int> RayCast(Vector2Int origin, Vector2Float direction, SpaceShip * spaceship, float maxDistance = FLT_MAX);
}

struct PhysicsUpdateContext
{
	float deltaTime;
};