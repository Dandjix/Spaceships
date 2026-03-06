#pragma once
#include "userInterface/GUIRect.h"
#include <functional>
//
// Created by timon on 3/3/26.
//

namespace GUI {
    class TextButton : public GUIRect{
    public:
        Event<> on_pressed;
    protected:
        std::function<bool()> is_clickable;
        std::string text;
        bool highlighted;
    public:
        TextButton(Anchor anchor, const Vector2Int &offset, int width, int height, const std::string&  text, bool highlighted = false, std::function<bool()> is_clickable = [](){return true;})
            : GUIRect(anchor, offset, width, height),text(text),highlighted(highlighted),is_clickable(std::move(is_clickable)) {
        }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;

        void update(const GUI_UpdateContext &context) override;

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override;
    };
}
