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

	virtual float get_weight()
	{
		return 1.0f;
	}

	/**
	 * Creates a visitor, which can be consumed by calling it with another physics entity, calling the right physics function
	 * @return a pointer to the visitor to be consumed
	 */
	virtual PhysicsUpdateVisitor * createVisitor()
	= 0;

	/**
	 * Calls the correct physics collision function using the visitor beore deleting it (To be clear : this function deletes the instance)
	 * /!\ You must call PhysicsEntity::consumeVisitor(visitor) AFTER your call to the correct method in the override.
	 * @param visitor the visitor
	 * @param space_ship the spaceship that both entities belong to
	 */
	virtual void consumeVisitor(PhysicsUpdateVisitor* visitor, SpaceShip* space_ship)=0;

	PhysicsEntity(Vector2Int position, std::optional<float> angle, Behavior * behavior) : Entity(position, angle, behavior){}

	void registerInSpaceship(SpaceShip* space_ship) override;

	void unregisterInSpacehip(SpaceShip* space_ship) override;
};



