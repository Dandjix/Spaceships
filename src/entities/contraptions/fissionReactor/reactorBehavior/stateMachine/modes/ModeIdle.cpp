//
// Created by timon on 2/19/26.
//

#include "ModeIdle.h"

#include "entities/contraptions/fissionReactor/controlRod/ControlRod.h"
#include "entities/contraptions/fissionReactor/reactor/FissionReactor.h"
#include "game/Update.h"

constexpr float ROD_EQUALITY_TOLERANCE = 0.05f;

using ModeIdle = Contraptions::FissionReactor::Behavior::ModeIdle;

int ModeIdle::get_opposite_rod_index(Reactor *reactor) const {
    int nb_rods = reactor->getControlRods().size();

    return (working_control_rod_index + nb_rods / 2) % nb_rods;
}

void ModeIdle::balance_rods(const UpdateContext &context, Reactor *reactor) {
    ControlRod *working = reactor->getControlRods().at(working_control_rod_index);
    ControlRod *opposite = reactor->getControlRods().at(get_opposite_rod_index(reactor));
    float difference = working->getProgress() - (1 - opposite->getProgress());
    const float rod_distance_this_frame = moment_percentage * context.deltaTime;

    // Clamp correction to not overshoot the balanced state
    float correction = std::copysign(std::min(std::abs(difference), rod_distance_this_frame), difference);

    // Shift both rods to reduce the difference:
    // working goes down, opposite goes up (mirroring cycling_animation's direction)
    working->setProgress(working->getProgress() - correction);
    opposite->setProgress(opposite->getProgress() - correction);
}

void ModeIdle::cycling_animation(Reactor *reactor, const UpdateContext &context) {
    if (reactor->getControlRods().at(working_control_rod_index)->getProgress() <= ROD_EQUALITY_TOLERANCE) {
        working_control_rod_index++;
        working_control_rod_index %= reactor->getControlRods().size();
        return;
    }

    ControlRod *working = reactor->getControlRods().at(working_control_rod_index);
    ControlRod *opposite = reactor->getControlRods().at(get_opposite_rod_index(reactor));

    const float rod_distance_this_frame = moment_percentage * context.deltaTime;

    working->setProgress(working->getProgress() - rod_distance_this_frame);
    opposite->setProgress(opposite->getProgress() + rod_distance_this_frame);
}

void ModeIdle::update(const UpdateContext &context, Reactor *reactor) {
    if (reactor->getControlRods().size() < 2)
        return; //cant do the cycle animation thingy if there are less than 2 control rods

    int opposite = get_opposite_rod_index(reactor);

    float diff = reactor->getControlRods().at(opposite)->getProgress()
                 - (1 - reactor->getControlRods().at(working_control_rod_index)->getProgress());

    if (std::abs(diff) > ROD_EQUALITY_TOLERANCE)
        balance_rods(context, reactor);

    else
        cycling_animation(reactor, context);
}
