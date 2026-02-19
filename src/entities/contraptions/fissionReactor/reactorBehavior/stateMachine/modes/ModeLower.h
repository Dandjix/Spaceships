#pragma once
#include "entities/contraptions/fissionReactor/reactorBehavior/stateMachine/Mode.h"
//
// Created by timon on 2/19/26.
//
namespace Contraptions::FissionReactor::Behavior {
    class ModeLower : public Mode {
    protected:
        float moment_percentage;
    public:
        explicit ModeLower(float moment_percentage) : moment_percentage(moment_percentage) {  }

        void update(const UpdateContext &context, Reactor *reactor) override;
    };
}
