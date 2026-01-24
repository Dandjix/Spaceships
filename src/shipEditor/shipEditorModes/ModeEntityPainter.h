//
// Created by timon on 1/24/26.
//

#pragma once
#include "ShipEditorMode.h"

namespace ShipEditorModes {
    class ModeEntityPainter : public ShipEditorMode {
    private:
        std::vector<Entity * > addedActiveEntities;
        std::vector<GUIRect * > addedEditorGUIElements;
    public:
        explicit ModeEntityPainter(ShipEditorStateMachine *state_machine)
            : ShipEditorMode(state_machine) {
        }

        void enter() override;

        void leave() override;
    };

}
