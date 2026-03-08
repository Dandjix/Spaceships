//
// Created by timon on 11/24/25.
//

#include "ShipEditorMode.h"
#include "ShipEditorStateMachine.h"
#include "shipEditor/EntityPlacer/EntityPlacement/interface/Interface.h"

void ShipEditorModes::ShipEditorMode::addActiveEntities(const std::vector<Entity *> &to_add) const {
    for (auto e: to_add)
        state_machine->activeEntities->insert(e);
}

void ShipEditorModes::ShipEditorMode::fakeKillActiveEntities(const std::vector<Entity *> &to_remove) const {
    for (auto e: to_remove) {
        state_machine->activeEntities->add_to_erasing_queue(e);
        e->on_killed.emit(nullptr);
    }
}

void ShipEditorModes::ShipEditorMode::addGUIElements(const std::vector<GUIRect *> &to_add) const {
    for (auto e: to_add)
        state_machine->editorGUIElements->insert(e);
}

void ShipEditorModes::ShipEditorMode::killGUIElements(const std::vector<GUIRect *> &to_remove) const {
    for (auto e: to_remove)
        e->kill(state_machine->editorGUIElements);
}

void ShipEditorModes::ShipEditorMode::enter() {
    added_entities = {};
    added_ui_elements = {};

    createEntitiesAndElements();

    addActiveEntities(added_entities);
    addGUIElements(added_ui_elements);
}

void ShipEditorModes::ShipEditorMode::leave() {
    fakeKillActiveEntities(added_entities);
    killGUIElements(added_ui_elements);

    added_entities = {};
    added_ui_elements = {};

    afterLeave();
}
