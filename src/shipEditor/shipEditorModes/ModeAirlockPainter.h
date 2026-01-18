//
// Created by timon on 12/19/25.
//

#pragma once
#include "ShipEditorMode.h"


namespace ShipEditorModes {
    class ModeAirlockPainter : public ShipEditorMode {
    private:
        // SpaceshipHooks *hooks;

        std::vector<Entity * > addedActiveEntities;
        std::vector<GUIRect * > addedEditorGUIElements;

    public:
        explicit ModeAirlockPainter(ShipEditorStateMachine * state_machine)
            : ShipEditorMode(state_machine) {
        }

        void enter() override;

        void leave() override;
    };
}
