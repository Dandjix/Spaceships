//
// Created by timon on 2/19/26.
//

#include "FissionReactorBehavior.h"

#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"

using FissionReactorBehavior = Contraptions::FissionReactor::FissionReactorBehavior;

constexpr float TOLERANCE = 10;

void FissionReactorBehavior::update(const UpdateContext &context, Reactor *reactor) {

    float output = reactor->getOutput();

    if (output > target_output + TOLERANCE) {
        state_machine.setMode(Behavior::StateMachine::LOWER);
    }
    else if (output < target_output - TOLERANCE) {
        state_machine.setMode(Behavior::StateMachine::RAISE);
    }
    else {
        state_machine.setMode(Behavior::StateMachine::IDLE);
    }

    state_machine.update(context, reactor);
}
