//
// Created by timon on 2/28/26.
//

#pragma once
#include "scripts/GUIStringPrompt.h"
#include "scripts/IValuePrompt.h"

namespace GUI::Prompts {
    class IntPrompt : public StringPrompt, public IValuePrompt<int> {
    private:
        static std::string valueToString(int int_value) { return std::to_string(int_value); }

        static int valueFromString(const std::string &string_value) { return std::stoi(string_value); }

    public:
        IntPrompt(Anchor anchor,
                  const GUIVector2Int &offset,
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


        [[nodiscard]] int getValue() const override { return valueFromString(value); }

        void setValue(int new_value) override { value = valueToString(new_value); }

        [[nodiscard]] [[nodiscard]] bool inputIsValid() const override {
            try {
                std::stoi(value);
            } catch (...) {
                return false;
            }
            return true;
        }
    };
}
