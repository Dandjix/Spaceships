#include "SpaceShip.h"
#include "Entity.h"

SpaceShip::SpaceShip(SpaceShipBlueprint* blueprint)
{

}

const std::unordered_set<Entity*>& SpaceShip::getEntities() const
{
	std::unordered_set<Entity*> set = std::unordered_set<Entity*>();
	return set;
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

