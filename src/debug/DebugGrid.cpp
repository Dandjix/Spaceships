
#include "DebugGrid.h"

#include "../math/Vectors.h"

#include <iostream>

DebugGrid::DebugGrid(int xOrigin, int yOrigin, int grid_resolution) : Entity(Vector2Int(0,0),0,nullptr)
{
    origin = {xOrigin, yOrigin};
    resolution = grid_resolution;
}

DebugGrid::~DebugGrid()
= default;


Vector2Int closest_point(Vector2Int position, int resolution)
{
    int closest_x = position.x % (resolution * Vectors::getFactor());
    int closest_y = position.y % (resolution * Vectors::getFactor());

    return {closest_x, closest_y};
}

void DebugGrid::debugRender(SDL_Renderer* renderer, const RenderingContext& context)
{
    Vector2Int closest_point_to_camera = closest_point(context.cameraPos,resolution);

    // render all lines in a radius visible to the camera. Assuming a square screen (worse scenario), we multiply by 1.42 to get the radius (the diagonal)
    int screen_radius = std::max(context.screenDimensions.x,context.screenDimensions.y)* 1.42;
    int world_radius = screen_radius * Vectors::getFactor();

    int nb_lines_and_columns = world_radius*2 / (resolution*Vectors::getFactor());

    Vector2Int start_x = closest_point_to_camera - Vector2Int(1,1) * world_radius;

    for (int i=0;i<=nb_lines_and_columns; i++)
    {
        for (int j = 0; j < nb_lines_and_columns; ++j)
        {

        }
    }

    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderLine(renderer,0,0,1000,1000);
}
