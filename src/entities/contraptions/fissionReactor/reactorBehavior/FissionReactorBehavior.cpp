//
// Created by timon on 2/19/26.
//

#include "FissionReactorBehavior.h"

#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"

using ReactorBehavior = Contraptions::FissionReactor::FissionReactorBehavior;

constexpr float TOLERANCE = 10;

// void ReactorBehavior::update(const UpdateContext &context,FissionReactor::Reactor *reactor) {
//     if (reactor->getOutput() < target_throughput - TOLERANCE)
//         state_machine.
//     else if (reactor->getOutput() > target_throughput + TOLERANCE)
//         lowerOutput(context,reactor);
//
//     else
//         idle(context,reactor);
//
//     state_machine.update(context,reactor);
//
// }
