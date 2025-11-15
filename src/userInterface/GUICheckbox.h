//
// Created by timon on 11/14/25.
//

#pragma once
#include <utility>

#include "GUI.h"
#include "GUIRect.h"
#include "../math/Vectors.h"


class GUICheckbox : public GUIRect
{
private:
    bool value;
    std::function<void(bool)> onValueChanged;

public:
    GUICheckbox(
        Anchor anchor,
        Vector2Int offset,
        std::function<void(bool)> onValueChangedCallback,
        bool initial_value = false
        ) : GUIRect(anchor,offset,20,20)
    {
        value = initial_value;
        onValueChanged = std::move(onValueChangedCallback);
    }
    bool get_value() const {return value;}

    void handleEvent(const SDL_Event& event) override;

    void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const override;
};
