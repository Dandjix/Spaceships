//
// Created by timon on 11/16/25.
//
#include "SpaceshipTiles.h"

#include <queue>

#include "instance/Instance.h"
#include "instance/Rooms/Room.h"
#include "spaceships/SpaceShip.h"
#include "spaceships/TileRendering.h"

int SpaceshipTiles::size_x() const {
    return tiles.size();
}

int SpaceshipTiles::size_y() const {
    int size_y;
    if (tiles.size() > 0) {
        size_y = tiles[0].size();
    } else {
        size_y = 0;
    }
    return size_y;
}

int SpaceshipTiles::minimum_x() const {
    return 0;
}

int SpaceshipTiles::maximum_x() const {
    return size_x();
}

int SpaceshipTiles::minimum_y() const {
    return 0;
}

int SpaceshipTiles::maximum_y() const {
    return size_y();
}

Tile SpaceshipTiles::get_tile(int x, int y) const {
    if (x < minimum_x() || x >= maximum_x() || y < minimum_y() || y >= maximum_y()) {
        return Tile::Void;
    }
    return tiles[x][y];
}

void SpaceshipTiles::render(SDL_Renderer *renderer, const RenderingContext &context, std::vector<Room *> to_render) const {
    for (const Room *room: to_render) {
        Vector2Int TL, BR;
        room->Encompassing(TL, BR);


        for (int x = TL.x; x <= BR.x; x++) {
            for (int y = TL.y; y <= BR.y; y++) {
                if (!room->IncludesTilePosition(x, y)) {
                    continue;
                }

                Tile tile = context.instance->space_ship->spaceship_tiles->get_tile(x, y);

                if (tile == Tile::Void) {
                    continue;
                }
                //SDL_Log("tile : %s",Tiles::nameFromTile(tile).c_str());
                TileRendering::renderTile(renderer, context, tile, x, y);
            }
        }
    }
}
