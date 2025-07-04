#include "Physics.h"
#include "../spaceships/SpaceShip.h"
#include "../math/Vectors.h"

std::optional<Vector2Int> Physics::RayCast(Vector2Int origin, Vector2Float direction, SpaceShip* spaceship, float maxDistance)
{
    direction.normalize();
    int xDir, yDir;
    if (direction.x == 0) xDir = 0;
    else if (direction.x > 0)xDir = 1;
    else xDir = -1;

    if (direction.y == 0) yDir = 0;
    else if (direction.y > 0)yDir = 1;
    else yDir = -1;

    const std::vector<std::vector<Tile>> & tiles = spaceship->getBlueprintTiles();

    float Sx = std::sqrtf(1 + std::powf(direction.y / direction.x, 2));
    float Sy = std::sqrtf(1 + std::powf(direction.x / direction.y, 2));
    Vector2Int tileCoords;
    Vector2Int intersection;

    int terminator = 0;
    do
    {
        int Ax, Ay;
        int snappedX = Vectors::getFactor() - origin.x % Vectors::getFactor();
        int snappedY = Vectors::getFactor() - origin.y % Vectors::getFactor();

        Ax = origin.x - snappedX;
        if (xDir < 0)
            Ax = Vectors::getFactor() - Ax;

        Ay = origin.y - snappedY;
        if (yDir < 0)
            Ay = Vectors::getFactor() - Ay;



        float Vx = Ax * Sx;
        float Vy = Ay * Sy;

        //first loop logic
        if (Vx == Vy)
        {
            snappedX += xDir * Vectors::getFactor();
            snappedY += yDir * Vectors::getFactor();
            intersection = Vectors::toVector2Int(direction * Vx);
            SDL_Log("bruh perfect diagonal");

        }
        else if (Vx < Vy)
        {
            snappedX += xDir * Vectors::getFactor();
            intersection = Vectors::toVector2Int(direction * Vx);
        }
        else
        {
            snappedY += yDir * Vectors::getFactor();
            intersection = Vectors::toVector2Int(direction * Vy);
        }
        if ((origin - intersection).length() > maxDistance)
            return std::nullopt;

        tileCoords = Vector2Int(snappedX, snappedY) / Vectors::getFactor();

        if(tileCoords.x < 0 || tileCoords.x >= tiles.size() || tileCoords.y < 0 || tileCoords.y >= tiles[0].size())
            return std::nullopt;

        if (tiles[tileCoords.x][tileCoords.y] == Tile::Wall)
            return intersection;

        terminator++;
    } while (terminator<1000);
    //SDL_Log("terminator reached");

    return std::nullopt;
}
