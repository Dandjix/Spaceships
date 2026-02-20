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
    public:
        enum ReactorBehaviorMode {
            WORKING,
            EMERGENCY_SHUTDOWN,
            POWERED_OFF
        };
    protected:

        float target_output;
        Behavior::StateMachine working_mode_state_machine;
        ReactorBehaviorMode behavior_mode;

    public:
        explicit FissionReactorBehavior(float target_throughput, ReactorBehaviorMode behavior_mode)
            : target_output(target_throughput),
              working_mode_state_machine(Behavior::StateMachine::ModeEnum::IDLE), behavior_mode(behavior_mode) {
        }

        void set_target_throughput(float target_throughput_param) { target_output = target_throughput_param; }


        void update(const UpdateContext &context, Reactor *reactor);

        nlohmann::json getBehaviorMode() {
            return behavior_mode;
        }
    };
}
