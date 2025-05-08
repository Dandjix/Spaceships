#include "SpaceShip.h"
#include "Entity.h"
#include <queue>
#include <unordered_set>

void SpaceShip::populateRooms()
{
	int width = blueprint->tiles.size();
	int height = blueprint->tiles[0].size();
	std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (shouldSkipTile(x, y, visited))
				continue;

			std::unordered_set<Vector2Int> connected = collectConnectedFloorTiles(x, y, visited);
			Room* newRoom = createRoomFromTiles(connected);
			rooms.addVertex(newRoom);
		}
	}
}

bool SpaceShip::shouldSkipTile(int x, int y, const std::vector<std::vector<bool>>& visited) const
{
	if (blueprint->tiles[x][y] != Tile::Floor)
		return true;

	if (visited[x][y])
		return true;

	for (Room* room : rooms.getVertices())
	{
		if (room->IncludesTilePosition(x, y))
			return true;
	}

	return false;
}

std::unordered_set<Vector2Int> SpaceShip::collectConnectedFloorTiles(int startX, int startY, std::vector<std::vector<bool>>& visited) const
{
	std::queue<Vector2Int> q;
	std::unordered_set<Vector2Int> connectedTiles;

	q.push(Vector2Int(startX, startY));
	visited[startX][startY] = true;

	std::vector<Vector2Int> directions = {
		Vector2Int(1, 0), Vector2Int(-1, 0),
		Vector2Int(0, 1), Vector2Int(0, -1)
	};

	while (!q.empty())
	{
		Vector2Int current = q.front();
		q.pop();
		connectedTiles.insert(current);

		for (const Vector2Int& dir : directions)
		{
			int nx = current.x + dir.x;
			int ny = current.y + dir.y;

			if (nx >= 0 && ny >= 0 && nx < blueprint->tiles.size() && ny < blueprint->tiles[0].size())
			{
				if (!visited[nx][ny] && blueprint->tiles[nx][ny] == Tile::Floor)
				{
					q.push(Vector2Int(nx, ny));
					visited[nx][ny] = true;
				}
			}
		}
	}

	return connectedTiles;
}

Room* SpaceShip::createRoomFromTiles(const std::unordered_set<Vector2Int>& tiles) const
{
	return new Room(tiles); // Assumes you have this constructor or similar method
}


Vector2Int SpaceShip::nextNonRoomCoords(int startX, int startY)
{
	for (int x = startX; x < blueprint->tiles.size(); x++)
	{
		for (int y = startY; y < blueprint->tiles[x].size(); y++)
		{
			Tile tile = blueprint->tiles[x][y];
			if (tile == Tile::Void || tile == Tile::Wall)
				continue;
			for (Room* room : rooms.getVertices())
			{
				if (!room->IncludesTilePosition(x, y))
					return (x,y);
			}
		}
	}
	return (-1, -1);
}

bool SpaceShip::roomsAreDone()
{
	int x = 0;
	int y = 0;
	for (std::vector<Tile> & column : blueprint->tiles)
	{
		for (Tile tile : column)
		{
			if(tile == Tile::Void || tile == Tile::Wall)
				continue;
			for (Room * room : rooms.getVertices())
			{
				if (!room->IncludesTilePosition(x, y))
					return false;
			}
			y++;
		}
		y=0;
		x++;
	}
	return true;
}

SpaceShip::SpaceShip(SpaceShipBlueprint* blueprint)
{
	this->blueprint = blueprint;
}

const std::unordered_set<Entity*>& SpaceShip::getEntities(EntityPriorityQueue queue) const
{
	return entities;
}

void SpaceShip::Dock(SpaceShip other)
{

}

void SpaceShip::registerEntities(std::initializer_list<Entity*> entities)
{
	for (Entity* e: entities)
	{
		this->entities.insert(e);
	}
}

void SpaceShip::unregisterEntities(std::initializer_list<Entity*> entities)
{
	for (Entity* e : entities)
	{
		this->entities.erase(e);
	}
}

