//
// Created by timon on 2/28/26.
//

#include "GUIIntPrompt.h"
#include <string>

std::string GUIIntPrompt::valueToString(int int_value) {
    return std::to_string(int_value);
}

int GUIIntPrompt::valueFromString(std::string string_value) {
    return std::stoi(string_value);
}

GUIIntPrompt::GUIIntPrompt(Anchor anchor, const Vector2Int &offset, int width, int height, SDL_Window *window,
    bool shown, int value, const std::string &placeholder, bool element_is_focused): GUIStringPrompt(anchor, offset, width, height, window, shown, valueToString(value), placeholder,
    element_is_focused) {
}

int GUIIntPrompt::getValue() { return valueFromString(value); }

void GUIIntPrompt::setValue(int new_value) { value = valueToString(new_value); }

bool GUIIntPrompt::inputIsValid() {
    try {
        int n = std::stoi(value);
    } catch (...) {
        return false;
    }
    return true;
}
