#pragma once
#include "stateMachine/StateMachine.h"
//
// Created by timon on 2/19/26.
//


struct UpdateContext;

namespace Contraptions::FissionReactor {
    class Reactor;
    class ControlRod;

    class FissionReactorBehavior {
    protected:
        float target_throughput;
        Behavior::StateMachine state_machine;

    public:
        explicit FissionReactorBehavior(float target_throughput)
            : target_throughput(target_throughput),
              state_machine(Behavior::StateMachine::ModeEnum::IDLE) {
        }

        void set_target_throughput(float target_throughput_param) { target_throughput = target_throughput_param; }


        void update(const UpdateContext &context, Reactor *reactor) { state_machine.update(context, reactor); }
    };
}
