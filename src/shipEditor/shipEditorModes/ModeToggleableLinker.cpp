//
// Created by timon on 1/25/26.
//

#include "ModeToggleableLinker.h"

#include "ShipEditorStateMachine.h"
#include "shipEditor/EntityPlacer/ToggleableLinker/ToggleableLinker.h"

inline void ModeToggleableLinker::createEntitiesAndElements() {

    auto * linker = new ToggleableLinker(ToggleableLinker::PickingActivator,state_machine->common->placed_entities);

    added_entities.push_back(linker);
}