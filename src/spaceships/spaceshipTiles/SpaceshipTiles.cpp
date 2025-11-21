//
// Created by timon on 11/16/25.
//
#pragma once

#include "SpaceshipTiles.h"


int SpaceshipTiles::size_x() const
{
    return tiles.size();
}

int SpaceshipTiles::size_y() const
{
    int size_y;
    if (tiles.size() > 0)
    {
        size_y = tiles[0].size();
    }
    else
    {
        size_y = 0;
    }
    return size_y;
}

int SpaceshipTiles::minimum_x() const
{
    return 0;
}

int SpaceshipTiles::maximum_x() const
{
    return size_x();
}

int SpaceshipTiles::minimum_y() const
{
    return 0;
}

int SpaceshipTiles::maximum_y() const
{
    return size_y();
}

Tile SpaceshipTiles::get_tile(int x, int y) const
{
    if (x < minimum_x() || x >= maximum_x() || y < minimum_y() || y >= maximum_y())
    {
        return Tile::Void;
    }
    return tiles[x][y];
}
