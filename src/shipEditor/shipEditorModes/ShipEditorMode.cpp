//
// Created by timon on 11/24/25.
//

#include "ShipEditorMode.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ShipEditorMode::addActiveEntities(const std::vector<Entity *> &to_add) const {
    for (auto e: to_add)
        state_machine->activeEntities->insert(e);
}

void ShipEditorModes::ShipEditorMode::removeActiveEntities(const std::vector<Entity *> &to_remove) const {
    for (auto e: to_remove)
        state_machine->activeEntities->add_to_erasing_queue(e);
}

void ShipEditorModes::ShipEditorMode::addGUIElements(const std::vector<GUIRect *> &to_add) const {
    for (auto e: to_add)
        state_machine->editorGUIElements->insert(e);
}

void ShipEditorModes::ShipEditorMode::removeGUIElements(const std::vector<GUIRect *> &to_remove) const {
    for (auto e: to_remove)
        state_machine->editorGUIElements->add_to_erasing_queue(e);
}
