#pragma once
#include "Entity.h"
#include "SpaceShip.h"
#include <unordered_set>
class Room
{
protected:
	std::unordered_set<Entity*> entities = {};
	SpaceShip* spaceship;
public :

	Room(SpaceShip* s)
	{
		spaceship = s;
	}

	void Add(Entity* entity)
	{
		entities.insert(entity);
	}

	void Remove(Entity* entity)
	{
		entities.erase(entity);
	}

	void Scan()
	{

	}
};