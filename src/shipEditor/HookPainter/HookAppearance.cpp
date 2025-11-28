//
// Created by timon on 11/27/25.
//

#include "HookAppearance.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "spaceships/Hooks/HookPoint.h"
#include "spaceships/Hooks/HookRegion.h"
#include "userInterface/fonts.h"

void displayLabel(SDL_Renderer *renderer, const RenderingContext &context, const std::string &name, Vector2Int world_position) {
    auto textColor = SDL_Color(0,255,0,255);

    SDL_Surface* surface = TTF_RenderText_Solid(fonts["lg"], name.c_str(), name.length(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    Vector2Float scaled_text_dimensions = (Vector2Float(surface->w,surface->h)*0.5f)/context.camera_info.cameraScale;

    Vector2Float text_position =
            context.camera_info.worldToScreenPoint(
                world_position
            ) - scaled_text_dimensions*0.5f;

    SDL_FRect textRect;
    textRect.w = scaled_text_dimensions.x;
    textRect.h = scaled_text_dimensions.y;
    textRect.x = text_position.x;
    textRect.y = text_position.y;

    SDL_SetRenderDrawColor(renderer,50,50,50,255);
    SDL_RenderFillRect(renderer,&textRect);

    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void HookAppearance::render(SDL_Renderer *renderer, const RenderingContext &context) {

    for (const auto [name,region]: hooks->regions) {
        std::vector corners = {
            context.camera_info.worldToScreenPoint(region->TL),
            context.camera_info.worldToScreenPoint({region->TL.x, region->TL.y + region->dimensions.y}),
            context.camera_info.worldToScreenPoint(region->TL + region->dimensions),
            context.camera_info.worldToScreenPoint({region->TL.x + region->dimensions.x,region->TL.y})
        };

        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        for (int i = 0; i < corners.size(); ++i) {
            int next_i = (i+1)%corners.size();
            SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
        }

        displayLabel(renderer,context,name,region->TL + region->dimensions*0.5f);

    }

    for (const auto [name,point]: hooks->points) {

        std::vector corners = {
            context.camera_info.worldToScreenPoint(point->position + Vector2Int(32,32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(point->position + Vector2Int(-32,32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(point->position + Vector2Int(-32,-32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(point->position + Vector2Int(32,-32).scaleToWorldPosition())
        };

        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        for (int i = 0; i < corners.size(); ++i) {
            int next_i = (i+1)%corners.size();
            SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
        }



        displayLabel(renderer, context, name, point->position);

    }
}
