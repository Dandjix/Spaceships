#pragma once
#include "PhysicsEntity.h"
#include "../entities/RoundEntity.h"

class RoundPhysicsEntity : public RoundEntity, public PhysicsEntity
{public:
	RoundPhysicsEntity(
		Vector2Int position,
		std::optional<float> angle,
		float radius,
		Behavior* behavior
	)
		: Entity(position, angle, behavior)          // ⬅️ MUST call Entity once
		, RoundEntity(position, angle, radius, behavior)
		, PhysicsEntity(position, angle, behavior)
	{}

	// Inherited via PhysicsEntity
	void physicsUpdate(const PhysicsUpdateContext& context) override;
};