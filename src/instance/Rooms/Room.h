#pragma once
#include <unordered_set>
#include <climits>
#include "../../spaceships/Tile.h"

class Entity;

class Room
{
protected:
	std::unordered_set<Entity*> entities = {};
	/// <summary>
	/// first : TL position of the bounding box
	/// second : BR position of the bounding box
	/// </summary>
	std::vector<std::pair<Vector2Int,Vector2Int>> boundingBoxes;
public :

	const std::vector<std::pair<Vector2Int, Vector2Int>> getBoundingBoxes() const
	{
		return boundingBoxes;
	}

 	//Room(std::vector<std::pair<Vector2Int, Vector2Int>> boundingBoxes)
 	//{
 	//	this->boundingBoxes = boundingBoxes;
 	//}

	Room(const std::unordered_set<Vector2Int>& tiles)
	{
		// Simple bounding box from all tiles
		int minX = INT_MAX, minY = INT_MAX;
		int maxX = INT_MIN, maxY = INT_MIN;

		for (const Vector2Int& tile : tiles)
		{
			minX = std::min(minX, tile.x);
			minY = std::min(minY, tile.y);
			maxX = std::max(maxX, tile.x);
			maxY = std::max(maxY, tile.y);
		}
		
		Vector2Int TL,BR;
		TL = Vector2Int(minX-1, minY-1);
		BR = Vector2Int(maxX+2, maxY+2);

		boundingBoxes.push_back({ TL, BR - TL});
	}

	void Add(Entity* entity)
	{
		entities.insert(entity);
	}

	void Remove(Entity* entity)
	{
		entities.erase(entity);
	}

	bool IncludesTilePosition(int x, int y) const
	{
		for (auto [topLeft, dimensions] : boundingBoxes)
		{
			Vector2Int bottomRight = topLeft + dimensions;
			if (x >= topLeft.x && x < bottomRight.x && y >= topLeft.y && y < bottomRight.y)
				return true;
		}
		return false;
	}

	bool IncludesWorldPosition(Vector2Int worldPosition) const
	{
		int factor = Vectors::getFactor() * Tiles::tileSizePx;

		Vector2Int coordinates = worldPosition / factor;
		return IncludesTilePosition(coordinates.x,coordinates.y);
	}

	void Encompassing(Vector2Int& TL, Vector2Int& BR) const
	{
		int minX = INT_MAX, minY = INT_MAX;
		int maxX = INT_MIN, maxY = INT_MIN;
		for (auto bb : boundingBoxes)
		{
			if (bb.first.x < minX)
				minX = bb.first.x;
			if (bb.first.y < minY)
				minY = bb.first.y;

			Vector2Int BR = bb.second + bb.first;
			if (BR.x > maxX)
				maxX = BR.x;
			if (BR.y > maxY)
				maxY = BR.y;
		}

		TL = Vector2Int(minX, minY);
		BR = Vector2Int(maxX-1, maxY-1);
	}
};
