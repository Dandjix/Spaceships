//
// Created by timon on 11/24/25.
//

#pragma once
#include "ShipEditorMode.h"
#include <vector>

namespace ShipEditorModes {

    class ModeHookPainter : public ShipEditorMode {
    private:
        std::vector<Entity * > activeEntities;
        std::vector<GUIRect * > editorGUIElements;
    public:
        explicit ModeHookPainter(ShipEditorStateMachine *state_machine)
            : ShipEditorMode(state_machine) {
        }

        void enter() override;

        void leave() override;
    };

}
