//
// Created by timon on 2/28/26.
//

#pragma once
#include "scripts/GUIStringPrompt.h"
#include "scripts/IValuePrompt.h"

namespace GUI::Prompts {
    class FloatPrompt : public StringPrompt, public IValuePrompt<float> {
    private:
        static std::string valueToString(float float_value) { return std::to_string(float_value); }

        static float valueFromString(const std::string &string_value) { return std::stof(string_value); }

    public:
        FloatPrompt(Anchor anchor,
                     const Vector2Int &offset,
                     int width,
                     int height,
                     SDL_Window *window,
                     bool shown,
                     int value,
                     const std::string &placeholder = "42",
                     bool element_is_focused = true)
            : StringPrompt(anchor, offset, width, height, window, shown,
                              valueToString(value), placeholder,
                              element_is_focused) {
        }


        [[nodiscard]] float getValue() const override { return valueFromString(value); }

        void setValue(float new_value) override { value = valueToString(new_value); }

        [[nodiscard]] bool inputIsValid() const override {
            try {
                std::stof(value);
            } catch (...) {
                return false;
            }
            return true;
        }
    };
}
