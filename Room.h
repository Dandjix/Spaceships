#pragma once
#include "Entity.h"
#include <unordered_set>
#include "Tile.h"
class Room
{
protected:
	std::unordered_set<Entity*> entities = {};

	std::vector<std::pair<Vector2Int,Vector2Int>> boundingBoxes;
public :
	Room(std::vector<std::pair<Vector2Int, Vector2Int>> boundingBoxes)
	{
		this->boundingBoxes = boundingBoxes;
	}

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

		boundingBoxes.push_back({ Vector2Int(minX, minY), Vector2Int(maxX + 1, maxY + 1) });
	}

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
		for (auto [topLeft, bottomRight] : boundingBoxes)
		{
			if (x >= topLeft.x && x < bottomRight.x && y >= topLeft.y && y < topLeft.y)
				return true;
		}
		return false;
	}

	bool IncludesWorldPosition(Vector2Int worldPosition)
	{
		int factor = Vectors::getFactor() * Tiles::tileSizePx;

		Vector2Int coordinates = worldPosition / factor;
		return IncludesTilePosition(coordinates.x,coordinates.y);
	}

	void Scan()
	{

	}
};