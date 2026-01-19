//
// Created by timon on 12/19/25.
//

#include "ModeAirlockPainter.h"

#include "ShipEditorStateMachine.h"
#include "shipEditor/AirlockPainter/AirlockDeleter.h"
#include "shipEditor/AirlockPainter/AirlockPainterManager.h"
#include "shipEditor/AirlockPainter/OrientationChanger.h"
#include "userInterface/elements/prompts/GUITextPrompt.h"

#include "shipEditor/AirlockPainter/PainterStates/Painter.h"
#include "shipEditor/HookPainter/HookAppearance.h"
#include "spaceships/SpaceShipBlueprint.h"
#include "spaceships/Hooks/airlock/Airlock.h"
#include "userInterface/elements/GUI/GUILabel.h"
#include "userInterface/elements/GUI/GUIList.h"

void ShipEditorModes::ModeAirlockPainter::enter() {
    addedActiveEntities = {};
    addedEditorGUIElements = {};

    auto airlock_name_dialog = new GUITextPrompt(Anchor::Center, {0, 0}, 400, 100, state_machine->window, false);
    addedEditorGUIElements.push_back(airlock_name_dialog);

    airlock_name_dialog->on_focused_change.subscribe([airlock_name_dialog](bool focused) {
        if (!focused) {
            airlock_name_dialog->hide();
        }
    });

    auto *airlock_painter = new AirlockPainter::Painter(AirlockPainter::Mode::Off, AirlockPainter::Precision::Center,
                                                        Airlock::Orientation::West);

    airlock_painter->promptForName = [airlock_name_dialog,airlock_painter]() {
        airlock_name_dialog->on_confirm.clear();
        airlock_name_dialog->on_confirm.subscribe([airlock_painter](const std::string &name) {
            airlock_painter->confirmPlacement(name);
        });

        if (!airlock_name_dialog->shown) {
            airlock_name_dialog->show();
            airlock_name_dialog->setFocused(true);
        }
    };

    airlock_painter->on_airlock_placed.subscribe(
        [this](const std::string &name, Vector2Int TL, Vector2Int dimensions, Airlock::Orientation orientation) {
            state_machine->common->blueprint->hooks.addAirlock(name, new Airlock(TL, dimensions, orientation));
        });

    addedActiveEntities.push_back(airlock_painter);

    auto airlock_deleter = new AirlockDeleter::AirlockDeleter(&state_machine->common->blueprint->hooks, false);
    addedActiveEntities.push_back(airlock_deleter);

    auto action_list = new GUIList(Anchor::TL, {100, 0}, 160, GUI_Fill, std::vector<std::string>{
                                       "Snap Center",
                                       "Snap Intersection",
                                       "Snap Off",
                                       "Paint Airlock",
                                       "Delete Airlock"
                                   },
                                   [airlock_painter,airlock_deleter](auto option) {
                                       if (option == "Snap Center") {
                                           airlock_painter->setPrecision(AirlockPainter::Precision::Center);
                                       } else if (option == "Snap Intersection") {
                                           airlock_painter->setPrecision(AirlockPainter::Precision::Intersection);
                                       } else if (option == "Snap Off") {
                                           airlock_painter->setPrecision(AirlockPainter::Precision::Free);
                                       } else if (option == "Paint Airlock") {
                                           airlock_painter->setMode(AirlockPainter::Mode::Airlock);
                                           airlock_deleter->disable();
                                       } else if (option == "Paint Off") {
                                           airlock_painter->setMode(AirlockPainter::Mode::Off);
                                       } else if (option == "Delete Airlock") {
                                           airlock_painter->setMode(AirlockPainter::Mode::Off);
                                           airlock_deleter->enable();
                                       }
                                   }
    );

    addedEditorGUIElements.push_back(action_list);

    auto appearance = new HookAppearance(&state_machine->common->blueprint->hooks);
    addedActiveEntities.push_back(appearance);

    auto orientationLabel = new GUILabel(Anchor::TR, {0, 0}, 100, 25, "", {0, 0, 255, 255}, fonts["sm"],
                                         QueueOrder::LAST);

    addedEditorGUIElements.push_back(orientationLabel);
    {
        auto orientationLabelBackward =
                std::function<std::string(Airlock::Orientation)>(
                    [](Airlock::Orientation orientation) -> std::string {
                        switch (orientation) {
                            case Airlock::Orientation::North:
                                return "North";
                            case Airlock::Orientation::South:
                                return "South";
                            case Airlock::Orientation::East:
                                return "East ->";
                            case Airlock::Orientation::West:
                                return "West <-";
                            default:
                                return "Unknown";
                        }
                    }
                );
        orientationLabel->text = orientationLabelBackward(airlock_painter->getOrientation());
        airlock_painter->on_orientation_changed.subscribe(
            [orientationLabel,orientationLabelBackward](Airlock::Orientation orientation) {
                orientationLabel->text = orientationLabelBackward(orientation);
            });
    }

    auto orientation_changer = new AirlockPainter::OrientationChanger(airlock_painter);
    addedActiveEntities.push_back(orientation_changer);

    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}


void ShipEditorModes::ModeAirlockPainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
