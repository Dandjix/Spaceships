#pragma once
#include "SpaceShipBlueprint.h"

class SpaceShip
{
protected:
	unsigned int xSize;
	unsigned int ySize;
public:
	SpaceShip(SpaceShipBlueprint * blueprint);
};