#include "Physics.h"
#include "../spaceships/SpaceShip.h"
#include "../math/Vectors.h"


// Sx = sqrt(1 + (dx/dy)²)
// Sy = sqrt(1 + (dy/dx)²)

const float step = 1.0f;

std::optional<Vector2Int> Physics::RayCast(
    Vector2Int origin,
    Vector2Float direction,
    SpaceShip* spaceship,
    float maxDistance)
{

    return std::nullopt;
}


std::optional<Vector2Int> NaiveRaycast(
    Vector2Int origin,
    Vector2Float direction,
    SpaceShip* spaceship,
    float maxDistance)
{
    const std::vector<std::vector<Tile>> & tiles = spaceship->getBlueprintTiles();
    Vector2Int tiles_dimentions;
    tiles_dimentions.x = tiles.size();
    tiles_dimentions.y = tiles[0].size();

    direction.normalize();
    Vector2Float step_vector = direction * step;

    float travelled = 0;
    bool hit = false;

    Vector2Float relative_position = {0,0};

    while (travelled < maxDistance)
    {
        relative_position += step_vector;
        travelled += step;

        Vector2Int world_position = origin + Vectors::toVector2Int(relative_position);

        Vector2Int tile_coordinates = world_position / static_cast<float>(Vectors::getFactor()*Tiles::tileSizePx);

        if (tile_coordinates.x < 0 || tile_coordinates.y < 0 || tile_coordinates.x >= tiles_dimentions.x || tile_coordinates.y >= tiles_dimentions.y)
        {
            return std::nullopt;
        }

        if (tiles[tile_coordinates.x][tile_coordinates.y] == Tile::Wall)
        {
            return world_position;
        }
    }
    return std::nullopt;
}
