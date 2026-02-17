#pragma once
#include <vector>
#include <SDL3/SDL.h>

#include "entities/scripts/Entity.h"
#include "entities/scripts/ShortLivedEntity.h"
#include "math/AdjacencyListGraph.h"
#include "math/Vectors.h"
#include "spaceships/spaceshipTiles/SpaceshipTiles.h"
//
// Created by timon on 2/15/26.
//


struct RenderingContext;
class Room;

class SpaceshipRooms : public ShortLivedEntity {
private:
    AdjacencyListGraph<Room *> rooms;

    SpaceshipTiles *spaceship_tiles;

    Room *createRoomFromTiles(const std::unordered_set<Vector2Int> &tiles) const;

    Vector2Int nextNonRoomCoords(int startX = 0, int startY = 0);

    void renderRooms(SDL_Renderer *renderer, const RenderingContext &context,
                     const std::vector<Room *> &to_render) const;

    bool roomsAreDone() const;

public:
    SpaceshipRooms(SpaceshipTiles *spaceship_tiles)
        : ShortLivedEntity({0, 0}, 0), spaceship_tiles(spaceship_tiles) {
    }

    ~SpaceshipRooms();

    bool shouldSkipTile(int x, int y,
                        const std::vector<std::vector<bool> > &visited) const;

    std::unordered_set<Vector2Int>
    collectConnectedFloorTiles(int startX, int startY,
                               std::vector<std::vector<bool> > &visited) const;

    std::vector<Room *> getRooms(Room *origin, int distance) {
        auto connected = rooms.connected(origin, distance);

        std::vector<Room *> room_vector = std::vector<Room *>(connected.begin(), connected.end());

        return room_vector;
    }

    std::vector<Room *> getAll() {
        auto connected = rooms.getVertices();

        std::vector<Room *> room_vector = std::vector<Room *>(connected.begin(), connected.end());

        return room_vector;
    }

    void populateRooms();

    void render(SDL_Renderer *renderer, const RenderingContext &context) override;

    Entity *initializeRendering(const EntityRendering::Context &context) override { return this; }

    Entity *finalizeRendering(const EntityRendering::Context &context) override { return this; }
};
