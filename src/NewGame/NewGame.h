//
// Created by timon on 11/29/25.
//

#pragma once
#include <filesystem>

#include "behavior/PlayerBehavior.h"
#include "entities/CargoContainer.h"
#include "entities/Humanoid.h"
#include "LoadGame/GameState.h"
#include "spaceships/SaveAndLoadShips.h"
#include "spaceships/SpaceShip.h"

namespace NewGame {
    std::filesystem::path ConstructNewGame();
}
