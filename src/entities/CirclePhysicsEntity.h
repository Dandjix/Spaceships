#pragma once
#include "../physics/PhysicsEntity.h"
#include "../entities/RoundEntity.h"

class RoundPhysicsEntity : public PhysicsEntity, public RoundEntity

{
	// Inherited via PhysicsEntity
	void physicsUpdate(const PhysicsUpdateContext& context) override;
};