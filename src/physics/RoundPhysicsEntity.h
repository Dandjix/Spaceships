#pragma once
#include "PhysicsEntity.h"
#include "../entities/RoundEntity.h"

class RoundPhysicsEntity : public RoundEntity, public PhysicsEntity
{public:

	void beVisitedBy(PhysicsEntity * other, PhysicsUpdateVisitor * visitor, SpaceShip * space_ship) override
	{
		visitor->visitRound(other,this,space_ship);
	}

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