#pragma once
#include "PhysicsShape.h"
#include "../entities/Entity.h"
#include "../physics/Physics.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitor.h"

class RoundPhysicsShape;
class RectPhysicsShape;

class PhysicsEntity : public Entity
{
public:
	PhysicsShape * const shape;

public :
	PhysicsEntity(const Vector2Int& position, const std::optional<float>& angle, Behavior* behavior, PhysicsShape * shape)
		: Entity(position, angle, behavior), shape(shape)
	{
	}

	virtual float get_weight() { return 1.0f; }

	void registerInSpaceship(SpaceShip* space_ship) override;

	void unregisterInSpacehip(SpaceShip* space_ship) override;
};



