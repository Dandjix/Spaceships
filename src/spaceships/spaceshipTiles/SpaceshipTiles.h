//
// Created by timon on 11/16/25.
//

#pragma once

#include <vector>

#include "math/Vectors.h"
#include "spaceships/Tile.h"


class SpaceshipTiles
{
private:
    std::vector<std::vector<Tile>> tiles;
    Vector2Int offset;

public:
    int size_x() const;
    int size_y() const;

    int minimum_x() const;
    int maximum_x() const;

    int minimum_y() const;
    int maximum_y() const;

    Tile get_tile(int x, int y) const;

    SpaceshipTiles(const std::vector<std::vector<Tile>> & tiles) : tiles(tiles){}
};


