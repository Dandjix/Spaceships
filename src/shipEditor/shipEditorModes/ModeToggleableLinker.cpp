//
// Created by timon on 1/25/26.
//

#include "ModeToggleableLinker.h"

#include "ShipEditorStateMachine.h"
#include "shipEditor/EntityPlacer/ToggleableLinker/ToggleableLinker.h"

inline void ModeToggleableLinker::enter() {
    added_entities = {};
    added_ui_elements = {};

    ToggleableLinker * linker = new ToggleableLinker(ToggleableLinker::PickingActivator,state_machine->common->placed_entities);

    added_entities.push_back(linker);

    addActiveEntities(added_entities);
}

inline void ModeToggleableLinker::leave() {
    removeActiveEntities(added_entities);

    for (auto e: added_entities) {
        delete e;
    }

    added_entities = {};
}
