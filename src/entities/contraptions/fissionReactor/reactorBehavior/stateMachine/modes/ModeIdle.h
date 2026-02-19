#pragma once
#include "entities/contraptions/fissionReactor/reactorBehavior/stateMachine/Mode.h"
//
// Created by timon on 2/19/26.
//

namespace Contraptions::FissionReactor::Behavior {
    class ModeIdle : public Mode {
    protected:
        int working_control_rod_index;
        float moment_percentage;

        int get_opposite_rod_index(Reactor *reactor) const;

        void balance_rods(const UpdateContext &context, Reactor *reactor);

        void cycling_animation(Reactor *reactor, const UpdateContext &context);

    public:
        explicit ModeIdle(int working_control_rod_index, float moment_percentage)
            : working_control_rod_index(working_control_rod_index), moment_percentage(moment_percentage) {
        }

        void update(const UpdateContext &context, Reactor *reactor) override;
    };
}
