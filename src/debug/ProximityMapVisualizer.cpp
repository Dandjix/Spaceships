//
// Created by timon on 2/9/26.
//

#include "ProximityMapVisualizer.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "spaceships/SpaceShip.h"
#include "userInterface/fonts.h"

void Debug::ProximityMapVisualizer::render(SDL_Renderer *renderer, const RenderingContext &context) {
    for (auto [position,shapes]: space_ship->hash_proximity_map) {
        Vector2Int world_position = ((position * Tiles::tileSizePx) + Vector2Int(Tiles::tileSizePx/2,Tiles::tileSizePx/2)).scaleToWorldPosition();
        Vector2Float screen_position = context.camera_info.worldToScreenPoint(world_position);
        Vector2Float text_dimensions = {0,0};

        SDL_Color color;
        {
            if (shapes.size()<=1)
                color = SDL_Color(0,255,0,255);
            else if (shapes.size()==2)
                color = SDL_Color(128,255,0,255);
            else if (shapes.size()==3)
                color = SDL_Color(255,255,0,255);
            else if (shapes.size()==4)
                color = SDL_Color(255,128,0,255);
            else //if (shapes.size()>=5)
                color = SDL_Color(255,0,0,255);
        }
        {
            std::string text = std::format("[{}]", shapes.size());

            SDL_Surface *surface = TTF_RenderText_Solid(fonts["sm"], text.c_str(), text.length(), color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_FRect textRect;
            textRect.w = static_cast<float>(surface->w);
            textRect.h = static_cast<float>(surface->h);
            textRect.x = screen_position.x + (text_dimensions.x - textRect.w) / 2; // center horizontally
            textRect.y = screen_position.y + (text_dimensions.y - textRect.h) / 2; // center vertically

            SDL_RenderTexture(renderer, texture, nullptr, &textRect);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }

    }
}
