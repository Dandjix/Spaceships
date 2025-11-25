//
// Created by timon on 11/24/25.
//

#include "ShipEditorMode.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ShipEditorMode::addActiveEntities(const std::vector<Entity *> &to_add) {
    addItems(state_machine->activeEntities,to_add);
}

void ShipEditorModes::ShipEditorMode::removeActiveEntities(const std::vector<Entity *> &to_remove) {
    removeItems(state_machine->activeEntities,to_remove);
}

void ShipEditorModes::ShipEditorMode::addGUIElements(const std::vector<GUIRect *> &to_add) {
    addItems(state_machine->GUIElements,to_add);
}

void ShipEditorModes::ShipEditorMode::removeGUIElements(const std::vector<GUIRect *> &to_remove) {
    removeItems(state_machine->GUIElements,to_remove);
}
