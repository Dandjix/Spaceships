#pragma once
#include "../entities/Entity.h"
#include "../physics/Physics.h"

class PhysicsEntity : virtual public Entity
{
public :
	virtual void  physicsUpdate(const PhysicsUpdateContext & context){}

	PhysicsEntity(Vector2Int position, std::optional<float> angle, Behavior * behavior) : Entity(position, angle, behavior){}

	void registerInSpaceship(SpaceShip* space_ship) override;

	void unregisterInSpacehip(SpaceShip* space_ship) override;
};

inline void PhysicsEntity::registerInSpaceship(SpaceShip* space_ship)
{
	Entity::registerInSpaceship(space_ship);
	space_ship->physics_entities.insert(this);
}

inline void PhysicsEntity::unregisterInSpacehip(SpaceShip* space_ship)
{
	Entity::unregisterInSpacehip(space_ship);
	space_ship->physics_entities.erase(this);
}


