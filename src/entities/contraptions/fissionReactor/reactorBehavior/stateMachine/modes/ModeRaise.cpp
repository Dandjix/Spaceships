//
// Created by timon on 2/19/26.
//

#include "ModeRaise.h"

#include "entities/contraptions/fissionReactor/controlRod/ControlRod.h"
#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"

using ModeRaise = Contraptions::FissionReactor::Behavior::ModeRaise;

void ModeRaise::update(const UpdateContext &context, Reactor *reactor) {
    for (ControlRod *rod: reactor->getControlRods()) {
        float progress = rod->getProgress();
        rod->setProgress(progress - moment_percentage * context.deltaTime);
    }
}