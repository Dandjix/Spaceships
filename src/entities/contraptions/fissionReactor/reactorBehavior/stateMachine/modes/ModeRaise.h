#pragma once
//
// Created by timon on 2/19/26.
//


#pragma once
#include "entities/contraptions/fissionReactor/reactorBehavior/stateMachine/Mode.h"
//
// Created by timon on 2/19/26.
//
namespace Contraptions::FissionReactor::Behavior {
    class ModeRaise : public Mode {
    protected:
        float moment_percentage;

    public:
        explicit ModeRaise(float moment_percentage) : moment_percentage(moment_percentage) {
        }

        void update(const UpdateContext &context, Reactor *reactor) override;
    };
}
