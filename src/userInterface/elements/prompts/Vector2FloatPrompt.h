//
// Created by timon on 2/28/26.
//

#pragma once
#include "scripts/GUIStringPrompt.h"
#include "scripts/IValuePrompt.h"

namespace GUI::Prompts {
    class Vector2FloatPrompt : public StringPrompt, public virtual IValuePrompt<Vector2Float> {
    public:
        Vector2FloatPrompt(
            Anchor anchor,
            const Vector2Int &offset,
            int width,
            int height,
            SDL_Window *window,
            bool shown = true,
            const std::string &raw_value = "",
            const std::string &placeholder = "(0,0)",
            const std::string &annotation = "[Vector2Float]",
            bool element_is_focused = false
        )
            : StringPrompt(anchor,
                           offset,
                           width,
                           height,
                           window,
                           shown,
                           raw_value,
                           placeholder,
                           annotation,
                           element_is_focused) {
        }

        static std::string serializeValue(const Vector2Float value) {
            return std::format("({},{})", value.x, value.y);
        }

        static float parseVectorComponent(std::string component) {
            // removing all the characters we don't want
            std::string chars = "[]{}() ";
            for (char i: chars) {
                std::erase(component, i);
            }
            return std::stof(component);
        }

        static Vector2Float parseVector(std::string value) {
            if (std::count(value.begin(), value.end(), ',') != 1)
                throw std::invalid_argument(
                    std::format("{} cannot be converted to Vector2Float (there are not 1 ',')", value));

            std::size_t comma_pos = value.find(',');

            std::string x_string = value.substr(0, comma_pos + std::string(",").length());
            std::string y_string = value.substr(comma_pos + std::string(",").length(), value.length());

            try {
                return {parseVectorComponent(x_string), parseVectorComponent(y_string)};
            } catch (const std::invalid_argument) {
                throw std::invalid_argument(std::format(
                    "{} cannot be converted to Vector2Float invalid argument for parsing of components (invalid argument)",
                    value));
            } catch (const std::out_of_range) {
                throw std::invalid_argument(std::format(
                    "{} cannot be converted to Vector2Float invalid argument for parsing of components (out of range)",
                    value));
            }
        }

        [[nodiscard]] Vector2Float getValue() const override {
            Vector2Float parsed = parseVector(value);
            return parsed;
        }

        [[nodiscard]] bool inputIsValid() const override {
            try {
                parseVector(value);
            } catch (const std::invalid_argument &) {
                return false;
            }
            return true;
        }

        void setValue(Vector2Float new_value) override {
            value = serializeValue(new_value);
        }

        [[nodiscard]] GUIRect *asGUIRect() override { return this; }
    };
}
