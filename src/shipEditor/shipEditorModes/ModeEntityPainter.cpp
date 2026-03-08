//
// Created by timon on 1/24/26.
//

#include "ModeEntityPainter.h"

#include "ShipEditorStateMachine.h"
#include "shipEditor/EntityPlacer/EntityPlacer.h"
#include "shipEditor/EntityPlacer/EntityPlacement/interface/Interface.h"
#include "userInterface/elements/GUI/GUIList.h"


ShipEditorModes::ModeEntityPainter::ModeEntityPainter(ShipEditorStateMachine *state_machine): ShipEditorMode(state_machine) {
}

void ShipEditorModes::ModeEntityPainter::createEntitiesAndElements() {
    auto entity_painter = new EntityPlacement::EntityPlacer(
        state_machine->common->placed_entities,
        EntityPlacement::EntityFactory::getInstance(),
        state_machine->common->entity_placement_context->interface,
        state_machine->common->entity_rendering_context,
        EntityPlacement::Center,
        true
    );

    auto registry_options = new GUIList(Anchor::TR,
                                        {0, 0},
                                        300,
                                        GUI_Fill, EntityPlacement::EntityFactory::getInstance().getOptions(),
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

    added_entities.push_back(
        entity_painter
    );
    added_ui_elements.push_back(registry_options);
    added_ui_elements.push_back(precision_list);
}

void ShipEditorModes::ModeEntityPainter::afterLeave() {
    state_machine->common->entity_placement_context->interface->close();
}
