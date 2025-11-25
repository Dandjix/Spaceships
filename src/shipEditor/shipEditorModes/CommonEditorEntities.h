//
// Created by timon on 11/24/25.
//

#pragma once
#include <vector>

enum class Tile;
class ShipBuildingGrid;
class SpaceShipBlueprint;
class BlueprintEditorAppearance;

namespace ShipEditorModes {
    struct CommonEditorObjects {
        SpaceShipBlueprint * blueprint;
        ShipBuildingGrid * grid;
        BlueprintEditorAppearance * appearance;
        std::vector<std::vector<Tile>> * tiles;
    };
}
