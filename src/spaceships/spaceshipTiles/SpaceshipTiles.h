//
// Created by timon on 11/16/25.
//

#pragma once

#include <vector>
#include <SDL3/SDL_render.h>

#include "math/AdjacencyListGraph.h"
#include "math/Vectors.h"


enum class Tile;
struct RenderingContext;
class Room;

class SpaceshipTiles {
private:


private:
    std::vector<std::vector<Tile> > tiles;
    Vector2Int offset;

public:
    int size_x() const;

    int size_y() const;

    int minimum_x() const;

    int maximum_x() const;

    int minimum_y() const;

    int maximum_y() const;

    Tile get_tile(int x, int y) const;

    explicit SpaceshipTiles(const std::vector<std::vector<Tile> > &tiles) : tiles(tiles) {
    }

    nlohmann::json toJson() {
        nlohmann::json json = tiles;
        return json;
    }

    static SpaceshipTiles * fromJson(const nlohmann::json & json) {
        return new SpaceshipTiles(json);
    }

    void render(SDL_Renderer *renderer, const RenderingContext &context, std::vector<Room *> to_render) const;
};
