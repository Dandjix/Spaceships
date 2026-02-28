//
// Created by timon on 2/28/26.
//

#pragma once
#include "scripts/GUIStringPrompt.h"
#include "scripts/IGUIValuePrompt.h"


class GUIIntPrompt : public GUIStringPrompt, public IGUIValuePrompt<int> {
private:
    static std::string valueToString(int int_value);

    static int valueFromString(std::string string_value);

public:
    GUIIntPrompt(Anchor anchor,
                 const Vector2Int &offset,
                 int width,
                 int height,
                 SDL_Window *window,
                 bool shown,
                 int value,
                 const std::string &placeholder = "42",
                 bool element_is_focused = true);

    int getValue() override;

    void setValue(int new_value) override;

    bool inputIsValid() override;
};
