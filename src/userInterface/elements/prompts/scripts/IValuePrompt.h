//
// Created by timon on 2/27/26.
//

#pragma once
#include "userInterface/GUIRect.h"

namespace GUI::Prompts {
    template<typename T>
    class IValuePrompt {
    public:
        virtual ~IValuePrompt() = default;

        [[nodiscard]] virtual T getValue() const = 0;

        virtual void setValue(T value) = 0;

        [[nodiscard]] [[nodiscard]] virtual bool inputIsValid() const = 0;

        [[nodiscard]] virtual GUIRect * asGUIRect() = 0;
    };
}
