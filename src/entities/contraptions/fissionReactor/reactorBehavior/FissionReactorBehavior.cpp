//
// Created by timon on 2/19/26.
//

#include "FissionReactorBehavior.h"

#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"

using FissionReactorBehavior = Contraptions::FissionReactor::FissionReactorBehavior;

constexpr float TOLERANCE = 10;

void FissionReactorBehavior::update(const UpdateContext &context, Reactor *reactor) {
    switch (behavior_mode) {
        case WORKING: {
            float output = reactor->getOutput();
            if (output > target_output + TOLERANCE) {
                working_mode_state_machine.setMode(Behavior::StateMachine::LOWER);
            } else if (output < target_output - TOLERANCE) {
                working_mode_state_machine.setMode(Behavior::StateMachine::RAISE);
            } else {
                working_mode_state_machine.setMode(Behavior::StateMachine::IDLE);
            }

            working_mode_state_machine.update(context, reactor);
            return;
        }
        case EMERGENCY_SHUTDOWN: {
            working_mode_state_machine.setMode(Behavior::StateMachine::LOWER);
            working_mode_state_machine.update(context, reactor);
            return;
        }
        case POWERED_OFF: {
            //nothing happens when the reactor is powered off
            return;
        }
    }
}
