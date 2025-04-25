#pragma once

#include "GUIRect.h"
#include <SDL3/SDL.h>

class GUIList : public GUIRect
{
public:
    GUIList(Anchor anchor, Vector2Int offset, int width, int height)
        : GUIRect(anchor, offset, width, height) {
    }

protected:
    void renderTexture(SDL_Renderer* renderer, Vector2Int TLCorner, Vector2Int dimensions) const override;
};
