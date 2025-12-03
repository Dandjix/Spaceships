#pragma once
#include "shapes/PhysicsShape.h"
#include "../entities/scripts/Entity.h"
#include "../physics/Physics.h"

class PhysicsShape;

class PhysicsEntity : public Entity
{
public:
	PhysicsShape * const shape;

public :
	PhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, PhysicsShape * shape)
		: Entity(position, angle), shape(shape)
	{
	}

	~PhysicsEntity() override
	{
		delete shape;
	}

	virtual float get_weight() { return 1.0f; }

	void registerInSpaceship(SpaceShip* space_ship) override;

	void unregisterInSpacehip(SpaceShip* space_ship) override;
};



