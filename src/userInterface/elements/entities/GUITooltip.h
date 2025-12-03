//
// Created by timon on 11/29/25.
//

#pragma once
#include <utility>

#include "../../../entities/scripts/ShortLivedEntity.h"
#include "userInterface/GUIRect.h"


class GUITooltip : public GUIRect {
public:
    Vector2Float screen_position;
    std::string text;
    SDL_Color text_color;
    SDL_Color rect_color;
    bool enabled;


    explicit GUITooltip(const Vector2Float &screen_position,
               bool enabled = true,
               std::string text = "",
               SDL_Color text_color = SDL_Color(255, 255, 255, 255),
               SDL_Color rect_color = SDL_Color(0, 0, 0, 255))
        : GUIRect(Anchor::TL, {0,0},0,0),screen_position(screen_position), text(std::move(text)), text_color(text_color), rect_color(rect_color), enabled(enabled){
    }

    void enable();

    void disable();

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    void setTextColor(SDL_Color sdl_color){text_color = sdl_color;}

    void setText(std::string new_text){text = std::move(new_text);}

    void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;

    void setPosition(Vector2Float new_screen_position) {
        screen_position = new_screen_position;
    }
};
