//
// Created by timon on 2/15/26.
//

#include "SpaceshipRooms.h"

#include "Room.h"
#include "game/Rendering.h"
#include "math/Hash.h"
#include "spaceships/ConnectRoomGraph.h"
#include "spaceships/TileRendering.h"
#include <queue>


void SpaceshipRooms::render(SDL_Renderer *renderer, const RenderingContext &context) {
    {
        std::vector<Room*> visible = rooms.getVertices();

        renderRooms(renderer, context, visible);
    }

    for (Room *room: rooms.getVertices()) {
        if (!room->getBoundingBoxes().empty()) {
            std::pair<Vector2Int, Vector2Int> bb = room->getBoundingBoxes()[0];
            int r, g, b;
            Hash::getRandomColor(bb.first.x, bb.first.y, &r, &g, &b);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        }
        std::vector<std::pair<Vector2Int, Vector2Int> > boxes = room->getBoundingBoxes();
        for (const std::pair<Vector2Int, Vector2Int> &bb: boxes) {
            DebugRendering::drawWorldRoomBoundingBox(renderer, context, bb.first, bb.second);
        }
    }
}

// -- Rooms


void SpaceshipRooms::populateRooms() {
    int width = spaceship_tiles->size_x();
    int height = spaceship_tiles->size_y();

    std::vector visited(width, std::vector<bool>(height, false));

    for (int x = 0; x < spaceship_tiles->size_x(); ++x) {
        for (int y = 0; y < spaceship_tiles->size_y(); ++y) {
            if (shouldSkipTile(x, y, visited))
                continue;
            std::unordered_set<Vector2Int> connected = collectConnectedFloorTiles(x, y, visited);
            Room * newRoom = createRoomFromTiles(connected);
            rooms.addVertex(newRoom);
        }
    }

    ConnectRoomGraph::Connect(rooms);
}

bool SpaceshipRooms::roomsAreDone() const {
    for (int x = 0; x < spaceship_tiles->size_x(); x++) {
        for (int y = 0; y < spaceship_tiles->size_y(); ++y) {
            Tile tile = spaceship_tiles->get_tile(x, y);

            if (tile == Tile::Void || tile == Tile::Wall)
                continue;
            for (Room *room: rooms.getVertices()) {
                if (!room->IncludesTilePosition(x, y))
                    return false;
            }
        }
    }
    return true;
}

SpaceshipRooms::~SpaceshipRooms() {
    for (Room *vertex: rooms.getVertices()) {
        delete vertex;
    }
}

bool SpaceshipRooms::shouldSkipTile(int x, int y, const std::vector<std::vector<bool> > &visited) const {
    if (spaceship_tiles->get_tile(x, y) != Tile::Floor)
        return true;

    if (visited[x][y])
        return true;

    for (Room *room: rooms.getVertices()) {
        if (room->IncludesTilePosition(x, y))
            return true;
    }

    return false;
}

std::unordered_set<Vector2Int> SpaceshipRooms::collectConnectedFloorTiles(int startX, int startY,
                                                                     std::vector<std::vector<bool> > &visited) const {
    std::queue<Vector2Int> q;
    std::unordered_set<Vector2Int> connectedTiles;

    q.push(Vector2Int(startX, startY));
    visited[startX][startY] = true;

    std::vector<Vector2Int> directions = {
        Vector2Int(1, 0), Vector2Int(-1, 0),
        Vector2Int(0, 1), Vector2Int(0, -1)
    };

    while (!q.empty()) {
        Vector2Int current = q.front();
        q.pop();
        connectedTiles.insert(current);

        for (const Vector2Int &dir: directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (nx >= 0 && ny >= 0 && nx < spaceship_tiles->size_x() && ny < spaceship_tiles->size_y()) {
                if (!visited[nx][ny] && spaceship_tiles->get_tile(nx, ny) == Tile::Floor) {
                    q.emplace(nx, ny);
                    visited[nx][ny] = true;
                }
            }
        }
    }

    return connectedTiles;
}

Room * SpaceshipRooms::createRoomFromTiles(const std::unordered_set<Vector2Int> &tiles) const {
    return new Room(tiles);
}

Vector2Int SpaceshipRooms::nextNonRoomCoords(int startX, int startY) {
    for (int x = startX; x < spaceship_tiles->size_x(); x++) {
        for (int y = startY; y < spaceship_tiles->size_y(); y++) {
            Tile tile = spaceship_tiles->get_tile(x, y);
            if (tile == Tile::Void || tile == Tile::Wall)
                continue;
            for (Room *room: rooms.getVertices()) {
                if (!room->IncludesTilePosition(x, y))
                    return Vector2Int(x, y);
            }
        }
    }
    return Vector2Int(-1, -1);
}

void SpaceshipRooms::renderRooms(SDL_Renderer *renderer, const RenderingContext &context,
                            const std::vector<Room *> &to_render) const {
    for (const Room *room: to_render) {
        Vector2Int TL, BR;
        room->Encompassing(TL, BR);


        for (int x = TL.x; x <= BR.x; x++) {
            for (int y = TL.y; y <= BR.y; y++) {
                if (!room->IncludesTilePosition(x, y)) {
                    continue;
                }

                Tile tile = spaceship_tiles->get_tile(x, y);

                if (tile == Tile::Void) {
                    continue;
                }
                //SDL_Log("tile : %s",Tiles::nameFromTile(tile).c_str());
                TileRendering::renderTile(renderer, context, tile, x, y);
            }
        }
    }
}