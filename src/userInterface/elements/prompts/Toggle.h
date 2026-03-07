//
// Created by timon on 3/7/26.
//

#pragma once
#include "scripts/IValuePrompt.h"
#include "userInterface/elements/GUI/TextButton.h"

namespace GUI::Prompts {
    class Toggle : public TextButton, public virtual IValuePrompt<bool> {
    protected:
        bool value;

        static std::string computeLabel(bool value) {
            return value ? "Yes" : "No";
        }

    public:
        Toggle(Anchor anchor, const Vector2Int &offset, int width, int height, bool value = false)
            : TextButton(anchor, offset, width, height, computeLabel(value)), value(value) {
            on_pressed.subscribe([this]() {
                this->value = !this->value;
            });
        }

        [[nodiscard]] bool inputIsValid() const override { return true; }

        [[nodiscard]] GUIRect *asGUIRect() override { return this; }

        [[nodiscard]] bool getValue() const override { return value; }

        void setValue(bool new_value) override { value = new_value; }
    };
}
