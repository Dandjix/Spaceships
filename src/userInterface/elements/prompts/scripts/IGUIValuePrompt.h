//
// Created by timon on 2/27/26.
//

#pragma once

template<typename T>
class IGUIValuePrompt {
public:
    virtual ~IGUIValuePrompt() = default;

    virtual T getValue() = 0;
    virtual void setValue(T value) = 0;
    virtual bool inputIsValid() = 0;
};