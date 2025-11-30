#pragma once
#include "../math/Vectors.h"
#include "../spaceships/SpaceShip.h"
#include <cfloat>

namespace Physics
{
	class RaycastHitInfo
	{
	public:
		explicit RaycastHitInfo(
			bool hit,
			Vector2Int hit_world_position = {0, 0},
		    const std::vector<Vector2Int>& checked_positions = {})
		:
			hit(hit),
			hit_world_position(hit_world_position),
			checked_positions(checked_positions)
		{}

		bool hit;
		Vector2Int hit_world_position;
		std::vector<Vector2Int> checked_positions;
	};

	RaycastHitInfo RayCast(Vector2Int origin, Vector2Float direction, SpaceShip * spaceship, float maxDistance = FLT_MAX);

	std::vector<PhysicsEntity*> EntityPointCast(Vector2Int world_position, SpaceShip * space_ship);
}

struct PhysicsUpdateContext
{
	float deltaTime;
	SpaceShip * space_ship;
};