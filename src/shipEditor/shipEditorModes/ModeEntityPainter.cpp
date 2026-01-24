//
// Created by timon on 1/24/26.
//

#include "ModeEntityPainter.h"

#include "ShipEditorStateMachine.h"
#include "shipEditor/EntityPlacer/EntityPlacer.h"
#include "userInterface/elements/GUI/GUIList.h"


void ShipEditorModes::ModeEntityPainter::enter() {
    addedActiveEntities = {};
    addedEditorGUIElements = {};

    EntityPlacement::Registry * registry = new EntityPlacement::Registry();

    auto entity_painter = new EntityPlacement::EntityPlacer(
        registry,
        state_machine->common->placed_entities,
        EntityPlacement::Center,
        true
        );

    auto registry_options = new GUIList(Anchor::TR,
                                        {0, 0},
                                        300,
                                        GUI_Fill, registry->getOptions(),
                                        [entity_painter](const std::string &option) {
                                            entity_painter->setEntityToPlace(option);
                                        });

    auto precision_list = new GUIList(Anchor::TL,
                                 {100, 0},
                                 100,
                                 300,
                                 {
                                     "Center",
                                     "Half",
                                     "Quarter",
                                     "Tenth",
                                     "Free"
                                 },
                                 [entity_painter](const std::string &option) {
                                     EntityPlacement::Precision p;
                                     if (option == "Center")
                                         p = EntityPlacement::Center;
                                     else if (option == "Half")
                                         p = EntityPlacement::Half;
                                     else if (option == "Quarter")
                                         p = EntityPlacement::Quarter;
                                     else if (option == "Tenth")
                                         p = EntityPlacement::Tenth;
                                     else //if (option == "Free")
                                         p = EntityPlacement::Free;
                                     entity_painter->setPrecision(p);
                                 });

    addedActiveEntities.push_back(
        entity_painter
    );
    addedEditorGUIElements.push_back(registry_options);
    addedEditorGUIElements.push_back(precision_list);

    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}

void ShipEditorModes::ModeEntityPainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
