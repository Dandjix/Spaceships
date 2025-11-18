#include "Physics.h"
#include "../spaceships/SpaceShip.h"
#include "../math/Vectors.h"
#include "../spaceships/spaceshipTiles/SpaceshipTiles.h"

const float step = 1.0f;

std::optional<Vector2Int> NaiveRaycast(
    Vector2Int origin,
    Vector2Float direction,
    SpaceShip* spaceship,
    float maxDistance)
{
    const SpaceshipTiles & tiles = spaceship->getSpaceshipTiles();
    Vector2Int tiles_dimensions;
    tiles_dimensions.x = tiles.size_x();
    tiles_dimensions.y = tiles.size_y();

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

        if (tile_coordinates.x < 0 || tile_coordinates.y < 0 || tile_coordinates.x >= tiles_dimensions.x || tile_coordinates.y >= tiles_dimensions.y)
        {
            return std::nullopt;
        }

        if (tiles.get_tile(tile_coordinates.x,tile_coordinates.y) == Tile::Wall)
        {
            return world_position;
        }
    }
    return std::nullopt;
}
// Sx = sqrt(1 + (dx/dy)²)
// Sy = sqrt(1 + (dy/dx)²)



/**
 * Casts a ray that interracts with the tilemap
 * @param origin the origin, in world units
 * @param direction the direction vector. This is normalized in the function.
 * @param spaceship the spaceship whose tilemap we are going to check
 * @param maxDistance the max distance after which to give up
 * @return a Physics::RaycastHitInfo object
 */
Physics::RaycastHitInfo Physics::RayCast(
    Vector2Int origin,
    Vector2Float direction,
    SpaceShip* spaceship,
    float maxDistance)
{
    const auto& tiles = spaceship->getSpaceshipTiles();

    int mapW = tiles.size_x();

    int mapH = tiles.size_y();

    direction.normalize();

    std::vector<Vector2Int> checked_positions = {};

    // Convert pixel-space origin into tile-space floating coords
    const float tileSize = Tiles::tileSizePx * Vectors::getFactor();
    Vector2Float rayPos = {
        origin.x / tileSize,
        origin.y / tileSize
    };



    // Which tile are we in?
    Vector2Int vMapCheck = {
        (int)rayPos.x,
        (int)rayPos.y
    };

    // Precompute delta distances
    Vector2Float deltaDist = {
        std::abs(1.0f / direction.x),
        std::abs(1.0f / direction.y)
    };

    Vector2Int vStep;
    Vector2Float sideDist;

    // Setup step direction and initial edge distances
    if (direction.x < 0)
    {
        vStep.x = -1;
        sideDist.x = (rayPos.x - vMapCheck.x) * deltaDist.x;
    }
    else
    {
        vStep.x = 1;
        sideDist.x = (vMapCheck.x + 1.0f - rayPos.x) * deltaDist.x;
    }

    if (direction.y < 0)
    {
        vStep.y = -1;
        sideDist.y = (rayPos.y - vMapCheck.y) * deltaDist.y;
    }
    else
    {
        vStep.y = 1;
        sideDist.y = (vMapCheck.y + 1.0f - rayPos.y) * deltaDist.y;
    }

    float distance = 0;


    maxDistance /= tileSize;
    while (distance < maxDistance)
    {
        // Convert ray length back to pixel world-space
        Vector2Int world_position_to_check = {
            (int)(origin.x + direction.x * (distance * tileSize)),
            (int)(origin.y + direction.y * (distance * tileSize))
        };

        checked_positions.push_back(world_position_to_check);

        if (vMapCheck.x < 0 || vMapCheck.y < 0
            || vMapCheck.x >= mapW || vMapCheck.y >= mapH)
            return RaycastHitInfo(false,{0,0},checked_positions);

        if (tiles.get_tile(vMapCheck.x,vMapCheck.y) == Tile::Wall)
        {
            return RaycastHitInfo(true,world_position_to_check,checked_positions);
        }

        if (sideDist.x < sideDist.y)
        {
            vMapCheck.x += vStep.x;
            distance = sideDist.x;
            sideDist.x += deltaDist.x;
        }
        else
        {
            vMapCheck.y += vStep.y;
            distance = sideDist.y;
            sideDist.y += deltaDist.y;
        }
    }

    return RaycastHitInfo(false,{0,0},checked_positions);

}
