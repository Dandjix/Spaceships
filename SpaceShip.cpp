#include "SpaceShip.h"
#include "Entity.h"

void SpaceShip::populateRooms()
{

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

const std::unordered_set<Entity*>& SpaceShip::getEntities() const
{
	return entities;
}

void SpaceShip::Dock(SpaceShip other)
{

}

void SpaceShip::registerEntities(std::initializer_list<Entity*> entities)
{

}

const void SpaceShip::unregisterEntities(std::initializer_list<Entity*> entities)
{

}

