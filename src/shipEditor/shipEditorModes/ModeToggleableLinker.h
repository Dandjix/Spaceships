//
// Created by timon on 1/25/26.
//

#pragma once
#include "ShipEditorMode.h"


class ModeToggleableLinker : public ShipEditorModes::ShipEditorMode{
protected:
    std::vector<Entity *> added_entities;
    std::vector<GUIRect *> added_ui_elements;
public:
    explicit ModeToggleableLinker(ShipEditorModes::ShipEditorStateMachine *state_machine)
        : ShipEditorMode(state_machine) {
    }

    void enter() override;

    void leave() override;
};

