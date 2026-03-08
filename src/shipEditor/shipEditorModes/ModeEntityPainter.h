//
// Created by timon on 1/24/26.
//

#pragma once
#include "ShipEditorMode.h"

namespace ShipEditorModes {
    class ModeEntityPainter : public ShipEditorMode {

    public:
        explicit ModeEntityPainter(ShipEditorStateMachine *state_machine)
            : ShipEditorMode(state_machine) {
        }


        void createEntitiesAndElements() override;
    };

}
