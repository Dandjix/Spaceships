//
// Created by timon on 11/24/25.
//

#include "ShipEditorMode.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ShipEditorMode::addActiveEntities(const std::vector<Entity *> &to_add) const {
    state_machine->activeEntities->add(to_add);
}

void ShipEditorModes::ShipEditorMode::removeActiveEntities(const std::vector<Entity *> &to_remove) const {
    state_machine->activeEntitiesDeletionQueue->add(to_remove);
}

void ShipEditorModes::ShipEditorMode::addGUIElements(const std::vector<GUIRect *> &to_add) const {
    state_machine->editorGUIElements->add(to_add);
}

void ShipEditorModes::ShipEditorMode::removeGUIElements(const std::vector<GUIRect *> &to_remove) const {
    state_machine->editorGUIElementsDeletionQueue->add(to_remove);
}
