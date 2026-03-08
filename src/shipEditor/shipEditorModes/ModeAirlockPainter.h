//
// Created by timon on 12/19/25.
//

#pragma once
#include "ShipEditorMode.h"


namespace ShipEditorModes {
    class ModeAirlockPainter : public ShipEditorMode {

    public:
        explicit ModeAirlockPainter(ShipEditorStateMachine * state_machine)
            : ShipEditorMode(state_machine) {
        }

        void createEntitiesAndElements() override;
    };
}
