#pragma once
#include "Entity.h"
#include <unordered_set>
class Room
{
protected:
	std::unordered_set<Entity*> entities = {};
public :
	void Add(Entity* entity)
	{
		entities.insert(entity);
	}

	void Remove(Entity* entity)
	{
		entities.erase(entity);
	}

	bool IncludesTilePosition(int x, int y)
	{
		return false;
	}

	bool IncludesWorldPosition(Vector2Int worldPosition)
	{
		return false;
	}

	void Scan()
	{

	}
};