#pragma once
#include "PhysicsEntity.h"
#include "../entities/RoundEntity.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitorRound.h"

class RoundPhysicsEntity : public RoundEntity, public PhysicsEntity
{public:
	PhysicsUpdateVisitor * createVisitor() override
	{
		return  new PhysicsUpdateVisitorRound(this);
	}
	void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship) override
	{
		visitor->visitRound(this,space_ship);
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
	void physicsUpdate(const PhysicsUpdateContext& context) override
	{
	}
};
