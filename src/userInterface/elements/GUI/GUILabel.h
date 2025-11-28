//
// Created by timon on 11/15/25.
//

#pragma once
#include "userInterface/fonts.h"
#include "userInterface/GUIRect.h"

class GUILabel : public GUIRect
{
public:
    TTF_Font * font;
    std::string text;
    SDL_Color color;

    GUILabel(Anchor anchor,Vector2Int offset,int width, int height,std::string labelText, SDL_Color labelColor, TTF_Font * labelFont) : GUIRect(anchor,offset,width,height)
    {
        font = labelFont;
        text = labelText;
        color = labelColor;
    }

    void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const override;
};

