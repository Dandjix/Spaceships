#include "GUIList.h"

void GUIList::renderTexture(SDL_Renderer* renderer, Vector2Int TLCorner, Vector2Int dimensions) const
{
    SDL_FRect rect{
        static_cast<float>(TLCorner.x),
        static_cast<float>(TLCorner.y),
        static_cast<float>(dimensions.x),
        static_cast<float>(dimensions.y)
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &rect);
}
