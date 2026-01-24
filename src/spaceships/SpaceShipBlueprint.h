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


class Entity;

/// <summary>
/// contains the layout info of a spaceship as well as it's name, outside appearance and spawn points
/// </summary>
class SpaceShipBlueprint
{
	public :
		std::filesystem::path path;
		std::string pathToExterior;
		std::vector<std::vector<Tile>> tiles;
		std::vector<Entity*> entities;
		SpaceshipHooks hooks;

		SpaceShipBlueprint(
			std::filesystem::path path,
			std::string pathToExterior,
			std::vector<std::vector<Tile>> tiles,
			std::vector<Entity *> entities,
			const SpaceshipHooks& hooks)
		:
		path(std::move(path)),pathToExterior(std::move(pathToExterior)),tiles(std::move(tiles)),entities(std::move(entities)),hooks(hooks)
		{

		}

		[[nodiscard]] std::string dumps() const;

		void resize(Vector2Int newDimensions);

		void paint(int x, int y, Tile tileToPaint);

		static SpaceShipBlueprint *load(const std::filesystem::path &path);
		static SpaceShipBlueprint *loads(const std::string &from, std::filesystem::path path);

};