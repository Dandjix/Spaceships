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

		RaycastHitInfo() = default;

		bool hit;
		Vector2Int hit_world_position;
		std::vector<Vector2Int> checked_positions;
	};

	[[nodiscard]] RaycastHitInfo RayCast(Vector2Int origin, Vector2Float direction, SpaceshipTiles *spaceship_tiles, float maxDistance = FLT_MAX);

	[[nodiscard]] std::vector<PhysicsEntity*> EntityPointCast(Vector2Int world_position, Instances::Instance *world_instance);
}

struct PhysicsUpdateContext
{
	float deltaTime;
	Instances::Instance * world_instance;
};