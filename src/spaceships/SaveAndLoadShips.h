#pragma once
#include "../imported/tinyfiledialogs.h"
#include <fstream>

void SaveShip(const std::string &blueprint, const std::string & file_name = "untitled.json");

std::string LoadShip(std::string * path = nullptr);