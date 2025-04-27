#pragma once
#include <string>
#include <vector>
#include "SpaceShipBlueprint.h"
#include "json.hpp"
#include <iostream>
#include "Vectors.h"
enum class Tile
{
	Void,
	Floor,
	Wall,
	DoorH,
	DoorV,
};


/// <summary>
/// contains the layout info of a spaceship as well as it's name, outside appearance and spawn points
/// </summary>
class SpaceShipBlueprint
{
	public :
		std::string name;
		std::string pathToExterior;
		std::vector<std::vector<Tile>> tiles;

		SpaceShipBlueprint(std::string name,std::string pathToExterior,std::vector<std::vector<Tile>> tiles);

		std::string dumps();

		void resize(Vector2Int newDimensions);

		void paint(int x, int y, Tile tileToPaint);

		static SpaceShipBlueprint load(std::string path);
		static SpaceShipBlueprint loads(std::string from, std::string name);

};