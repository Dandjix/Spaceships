//
// Created by timon on 2/19/26.
//

#include "ModeLower.h"

#include "entities/contraptions/fissionReactor/controlRod/ControlRod.h"
#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"

using ModeLower = Contraptions::FissionReactor::Behavior::ModeLower;

void ModeLower::update(const UpdateContext &context, Reactor *reactor) {
    for (ControlRod *rod: reactor->getControlRods()) {
        float progress = rod->getProgress();
        rod->setProgress(progress + moment_percentage * context.deltaTime);
    }
}
