#include <queue>
#include <unordered_set>

#include "SpaceShip.h"
#include "../entities/Entity.h"
#include "TileRendering.h"
#include "../math/Hash.h"
#include "ConnectRoomGraph.h"
#include "spaceshipTiles/SpaceshipTiles.h"

void SpaceShip::populateRooms()
{
	int width = spaceship_tiles.size_x();
	int height = spaceship_tiles.size_y();

	std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));

	for (int x = 0 ; x < spaceship_tiles.size_x(); ++x)
	{
		for (int y = 0; y < spaceship_tiles.size_y(); ++y)
		{
			if (shouldSkipTile(x, y, visited))
				continue;
			std::unordered_set<Vector2Int> connected = collectConnectedFloorTiles(x, y, visited);
			Room* newRoom = createRoomFromTiles(connected);
			rooms.addVertex(newRoom);
		}
	}

	ConnectRoomGraph::Connect(rooms);
}

bool SpaceShip::shouldSkipTile(int x, int y, const std::vector<std::vector<bool>>& visited) const
{
	if (spaceship_tiles.get_tile(x,y) != Tile::Floor)
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

			if (nx >= 0 && ny >= 0 && nx < spaceship_tiles.size_x() && ny < spaceship_tiles.size_y())
			{
				if (!visited[nx][ny] && spaceship_tiles.get_tile(nx,ny) == Tile::Floor)
				{
					q.emplace(nx, ny);
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
	for (int x = startX; x < spaceship_tiles.size_x(); x++)
	{
		for (int y = startY; y < spaceship_tiles.size_y(); y++)
		{
			Tile tile = spaceship_tiles.get_tile(x,y);
			if (tile == Tile::Void || tile == Tile::Wall)
				continue;
			for (Room* room : rooms.getVertices())
			{
				if (!room->IncludesTilePosition(x, y))
					return Vector2Int(x,y);
			}
		}
	}
	return Vector2Int(-1, -1);
}

bool SpaceShip::roomsAreDone()
{
	for (int x = 0; x < spaceship_tiles.size_x();x++)
	{
		for (int y = 0; y < spaceship_tiles.size_y(); ++y)
		{
			Tile tile = spaceship_tiles.get_tile(x,y);

			if(tile == Tile::Void || tile == Tile::Wall)
				continue;
			for (Room * room : rooms.getVertices())
			{
				if (!room->IncludesTilePosition(x, y))
					return false;
			}
		}
	}
	return true;
}

SpaceShip::SpaceShip(SpaceShipBlueprint* blueprint) : spaceship_tiles(SpaceshipTiles(blueprint->tiles))
{
	populateRooms();
}

const SpaceshipTiles & SpaceShip::getSpaceshipTiles() const
{
	return spaceship_tiles;
}

void SpaceShip::renderExterior(SDL_Renderer* renderer, const RenderingContext& context)
{
	//TODO : do dis
}

void SpaceShip::renderRooms(SDL_Renderer * renderer, const RenderingContext& context, const std::vector<Room*> & rooms)
{
	for (const Room * room : rooms)
	{
		Vector2Int TL, BR;
		room->Encompassing(TL, BR);


		for (int x = TL.x; x <= BR.x; x++)
		{
			for (int y= TL.y; y <= BR.y; y++)
			{
				if (!room->IncludesTilePosition(x, y)) {
					continue;
				}

				Tile tile = spaceship_tiles.get_tile(x,y);

				if (tile == Tile::Void)
				{
					continue;
				}
				//SDL_Log("tile : %s",Tiles::nameFromTile(tile).c_str());
				TileRendering::renderTile(renderer, context, tile, x, y);
			}
		}
	}
}

void SpaceShip::renderInterior(SDL_Renderer* renderer, const RenderingContext& context)
{
	if (focusRoom)
	{
		auto visible = rooms.connected(focusRoom, 2);
		std::vector<Room*> visibleRoomsVector(visible.begin(), visible.end());
		visibleRoomsVector.push_back(focusRoom);
		//std::vector<Room*> visibleRoomsVector = this->rooms.getVertices();
		//SDL_Log("rendering %d rooms (%d)",visibleRoomsVector.size(), visible.size());
		renderRooms(renderer, context, visibleRoomsVector);
	}
	else
	{
	}

	for (Room * room : rooms.getVertices())
	{
		if (!room->getBoundingBoxes().empty())
		{
			std::pair < Vector2Int,Vector2Int > bb = room->getBoundingBoxes()[0];
			int r, g, b;
			Hash::getRandomColor(bb.first.x, bb.first.y, &r, &g, &b);
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		}
		std::vector<std::pair<Vector2Int,Vector2Int>> boxes = room->getBoundingBoxes();
		for (const std::pair<Vector2Int, Vector2Int>& bb : boxes)
		{

			DebugRendering::drawWorldRoomBoundingBox(renderer,context,bb.first,bb.second);
		}
	}
}

std::vector<Entity*> SpaceShip::getEntities(RoomDistance queue) const
{
	auto vec = std::vector<Entity*>(entities.begin(), entities.end());

	std::sort(vec.begin(), vec.end(), EntityComparison::compareEntities);

	return vec;
}

void SpaceShip::Dock(SpaceShip other)
{

}

void SpaceShip::registerEntities(std::initializer_list<Entity*> entities)
{
	for (Entity* e: entities)
	{
		this->entities.insert(e);
		e->onRegistered(this);
	}
}

void SpaceShip::unregisterEntities(std::initializer_list<Entity*> entities)
{
	for (Entity* e : entities)
	{
		this->entities.erase(e);
		e->onUnRegistered(this);
	}
}

void SpaceShip::update(const UpdateContext& context)
{
	if (!focusEntity)
	{
		//SDL_Log("no focus entity");
		focusRoom = nullptr;
		return;
	}

	for (Room * room : rooms.getVertices())
	{
		if (room->IncludesWorldPosition(focusEntity->getPosition()))
		{
			//SDL_Log("There is a room !");
			focusRoom = room;
			return;
		}
	}
	//SDL_Log("There is no room.");
	focusRoom = nullptr;
}

void SpaceShip::setFocusEntity(Entity* focusEntity)
{
	this->focusEntity = focusEntity;
}

bool EntityComparison::compareEntities(Entity* e1, Entity* e2)
{
	return e1->getQueueOrder() < e2->getQueueOrder();
}
