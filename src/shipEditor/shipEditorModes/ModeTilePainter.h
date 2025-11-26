//
// Created by timon on 11/24/25.
//

#pragma once
#include "ShipEditorMode.h"

namespace ShipEditorModes {

class ModeTilePainter : public ShipEditorMode {
private:
    std::vector<Entity * > addedActiveEntities;
    std::vector<GUIRect * > addedEditorGUIElements;
public:
    explicit ModeTilePainter(ShipEditorStateMachine *state_machine)
        : ShipEditorMode(state_machine) {
    }

    void enter() override;

    void leave() override;
};

}
