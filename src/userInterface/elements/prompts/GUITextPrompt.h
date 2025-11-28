//
// Created by timon on 11/27/25.
//

#pragma once
#include <utility>

#include "userInterface/GUIRect.h"


class GUITextPrompt : public GUIRect{
private:
    int frames_until_active = 0;

public:


    std::string placeholder;
    SDL_Window * window;
    std::string value;
    bool focused;
    bool shown;

    Event<std::string> on_confirm;
    Event<bool> on_focused_change;

    GUITextPrompt(
        Anchor anchor,
        const Vector2Int &offset,
        int width,
        int height,
        SDL_Window * window,
        bool shown,
        std::string value = "",
        std::string placeholder = "Type here ...",
        bool element_is_focused = true
    )
        :
    GUIRect(anchor, offset, width, height),
        window(window),
        placeholder(std::move(placeholder)),
        value(std::move(value)),
        shown(shown)
    {
        setFocused(element_is_focused);
    }

    void setFocused(bool new_focused);

    void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;

    bool is_inside(Vector2Float position) override {
        if (!shown)
            return false;

        return GUIRect::is_inside(position);
    }

    void show();

    void hide();

    void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;

    unsigned short int getQueueOrder() override {
        return 1;
    }

    void update(const GUI_UpdateContext &context) override;
};
