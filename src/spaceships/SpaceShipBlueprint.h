#pragma once
#include <string>
#include <utility>
#include <vector>

#include "SpaceShipBlueprint.h"
#include "../imported/json.hpp"
#include <iostream>
#include "../math/Vectors.h"
#include "Tile.h"
#include "Hooks/SpaceshipHooks.h"


/// <summary>
/// contains the layout info of a spaceship as well as it's name, outside appearance and spawn points
/// </summary>
class SpaceShipBlueprint
{
	public :
		std::string name;
		std::string pathToExterior;
		std::vector<std::vector<Tile>> tiles;
		SpaceshipHooks hooks;

		SpaceShipBlueprint(std::string name,std::string pathToExterior,std::vector<std::vector<Tile>> tiles, SpaceshipHooks hooks)
		:
		name(std::move(name)),pathToExterior(std::move(pathToExterior)),tiles(std::move(tiles)),hooks(std::move(hooks))
		{

		}

		[[nodiscard]] std::string dumps() const;

		void resize(Vector2Int newDimensions);

		void paint(int x, int y, Tile tileToPaint);

		static SpaceShipBlueprint load(std::string path);
		static SpaceShipBlueprint loads(std::string from, std::string name);

};