#pragma once
#include "../math/Vectors.h"
#include "../spaceships/SpaceShip.h"
#include <cfloat>

namespace Physics
{
	const unsigned short PHYSICS_LAYER_SHIPINTERIOR = 1;
	const unsigned short PHYSICS_LAYER_ENTITIES = 2;
	//const unsigned short PHYSICS_LAYER_SOME_LAYER = 4;
	//const unsigned short PHYSICS_LAYER_SOME_LAYER = 8;


	//inline static std::optional<Vector2Int> SegmentCast(Vector2Int origin, Vector2Int destination, SpaceShip * spaceship);

	std::optional<Vector2Int> RayCast(Vector2Int origin, Vector2Float direction, SpaceShip * spaceship, float maxDistance = FLT_MAX);
	//inline static std::optional<Vector2Int> RayCast(Vector2Int origin, float angle, SpaceShip * spaceship);
}

struct PhysicsUpdateContext
{
	float deltaTime;
};