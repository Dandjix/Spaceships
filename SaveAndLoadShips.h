#pragma once
#include "tinyfiledialogs.h"
#include <fstream>

void SaveShip(std::string blueprint);

std::string LoadShip(std::string * name = nullptr);