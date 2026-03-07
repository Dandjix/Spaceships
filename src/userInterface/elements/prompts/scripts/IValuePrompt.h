//
// Created by timon on 2/27/26.
//

#pragma once
#include "userInterface/GUIRect.h"

namespace GUI::Prompts {

    class IPrompt {
    public:
        virtual ~IPrompt() = default;

        [[nodiscard]] virtual bool inputIsValid() const = 0;

        [[nodiscard]] virtual GUIRect * asGUIRect() = 0;
    };


    template<typename T>
    class IValuePrompt : public IPrompt{
    public:
        [[nodiscard]] virtual T getValue() const = 0;

        virtual void setValue(T value) = 0;


    };
}
