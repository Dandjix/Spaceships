#pragma once
#include "PhysicsEntity.h"
#include "RoundEntity.h"

class RoundPhysicsEntity : public PhysicsEntity, public RoundEntity

{
	// Inherited via PhysicsEntity
	void physicsUpdate(const PhysicsUpdateContext& context) override;
};