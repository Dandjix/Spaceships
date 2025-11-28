//
// Created by timon on 11/27/25.
//

#pragma once
#include <utility>

#include "userInterface/GUIRect.h"


class GUITextPrompt : public GUIRect{
public:


    std::string placeholder;

    std::string value;
    bool focused;
    bool shown;

    Event<std::string> on_confirm;

    GUITextPrompt(
        Anchor anchor,
        const Vector2Int &offset,
        int width,
        int height,
        bool shown,
        std::string value = "",
        std::string placeholder = "Type here ...",
        bool focused = true
    )
        :
    GUIRect(anchor, offset, width, height),
        placeholder(std::move(placeholder)),
        value(std::move(value)),
        focused(focused),
        shown(shown) {}

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    bool is_inside(Vector2Float position) override {
        if (!shown)
            return false;

        return GUIRect::is_inside(position);
    }

    void show();

    void hide();

    void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;
};
