#pragma once
#include "../entities/Entity.h"
#include "../physics/Physics.h"
#include "PhysicsUpdateVisitor/PhysicsUpdateVisitor.h"

class RoundPhysicsEntity;
class RectPhysicsEntity;

class PhysicsEntity : virtual public Entity
{
public :
	virtual void  physicsUpdate(const PhysicsUpdateContext & context);

	virtual void beVisitedBy(PhysicsEntity * other, PhysicsUpdateVisitor * visitor, SpaceShip * space_ship) = 0;

	PhysicsEntity(Vector2Int position, std::optional<float> angle, Behavior * behavior) : Entity(position, angle, behavior){}

	void registerInSpaceship(SpaceShip* space_ship) override;

	void unregisterInSpacehip(SpaceShip* space_ship) override;
};



