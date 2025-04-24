#include "SpaceShipBlueprint.h"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

SpaceShipBlueprint::SpaceShipBlueprint(std::string name, std::string pathToExterior, std::vector<std::vector<Tile>> tiles)
{
	this->name = name;
	this->pathToExterior = pathToExterior;
	this->tiles = tiles;
}

std::string SpaceShipBlueprint::dumps()
{
	json dict;
	dict["pathToExterior"] = pathToExterior;
	dict["tiles"] = tiles;
	return dict.dump(4);
}

SpaceShipBlueprint SpaceShipBlueprint::loads(std::string from, std::string name)
{
	json dict = json::parse(from);
	std::string pathToExterior = dict["pathToExterior"];
	std::vector<std::vector<Tile>> tiles = dict["tiles"];
	return SpaceShipBlueprint(name, pathToExterior, tiles);
}

SpaceShipBlueprint SpaceShipBlueprint::load(std::string path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return loads(buffer.str(), path); // or some name you determine from path
}
