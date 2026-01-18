//
// Created by timon on 12/19/25.
//

#include "AirlockAppearance.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "shipEditor/HookPainter/HookDebugRendering.h"
#include "spaceships/Hooks/airlock/Airlock.h"
#include "userInterface/fonts.h"


void displayAirlockLabel(SDL_Renderer *renderer, const RenderingContext &context, const std::string &name, Vector2Int world_position) {
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

void AirlockAppearance::render(SDL_Renderer *renderer, const RenderingContext &context) {

    for (const auto [name,airlock]: hooks->airlocks) {
        std::vector corners = {
            context.camera_info.worldToScreenPoint(airlock->TL),
            context.camera_info.worldToScreenPoint({airlock->TL.x, airlock->TL.y + airlock->dimensions.y}),
            context.camera_info.worldToScreenPoint(airlock->TL + airlock->dimensions),
            context.camera_info.worldToScreenPoint({airlock->TL.x + airlock->dimensions.x,airlock->TL.y})
        };

        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        for (int i = 0; i < corners.size(); ++i) {
            int next_i = (i+1)%corners.size();
            SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
        }

        displayAirlockLabel(renderer,context,name,airlock->TL + airlock->dimensions*0.5f);

        HookDebugRendering::displayArrow(renderer,context,airlock->TL + airlock->dimensions*0.5f,airlock->orientation);
    }
}
