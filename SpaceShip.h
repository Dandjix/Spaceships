#pragma once
#include "SpaceShipBlueprint.h"
#include <unordered_set>
#include <initializer_list>
#include "Entity.h"

class SpaceShip
{
protected:
	std::unordered_set<Entity*> entities;
public:
	SpaceShip(SpaceShipBlueprint * blueprint);

	/// <summary>
	/// returns the entities (readonly)
	/// </summary>
	/// <returns></returns>
	const std::unordered_set<Entity*>& getEntities() const;

	/// <summary>
	/// docks an other ship to this ship. You will want to align them relatively well since this will snap the other ship. Undocking is future me's problem
	/// </summary>
	/// <param name="other"> The other ship</param>
	void Dock(SpaceShip other);

	/// <summary>
	/// adds one or more entries to the entities map
	/// </summary>
	void registerEntities(std::initializer_list<Entity*> entities);

	/// <summary>
	/// removes one or more entries from the entities map. Used for when the object is destroyed or if another ship undocks prolly idk
	/// </summary>
	/// <param name="entity"></param>
	/// <returns></returns>
	const void unregisterEntities(std::initializer_list<Entity*> entities);

};