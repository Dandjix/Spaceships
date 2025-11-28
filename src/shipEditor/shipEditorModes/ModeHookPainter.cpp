//
// Created by timon on 11/24/25.
//

#include "ModeHookPainter.h"

#include "../../userInterface/elements/GUI/GUICheckbox.h"
#include "ShipEditorStateMachine.h"
#include "shipEditor/HookPainter/RegionDeleter.h"
#include "shipEditor/HookPainter/HookAppearance.h"
#include "shipEditor/HookPainter/PointDeleter.h"
#include "shipEditor/HookPainter/PainterStates/Painter.h"
#include "spaceships/Hooks/HookRegion.h"
#include "spaceships/Hooks/HookPoint.h"
#include "userInterface/elements/GUI/GUIList.h"
#include "userInterface/elements/prompts/GUITextPrompt.h"

void ShipEditorModes::ModeHookPainter::enter() {

    addedActiveEntities = {};
    addedEditorGUIElements = {};

    auto hook_name_dialog = new GUITextPrompt(Anchor::Center,{0,0},400,100,state_machine->window,false);
    addedEditorGUIElements.push_back(hook_name_dialog);

    hook_name_dialog->on_focused_change.subscribe([hook_name_dialog](bool focused) {
        if (!focused) {
            hook_name_dialog->hide();
        }
    });

    auto * hook_painter = new HookPainter::Painter(HookPainter::Mode::Off,HookPainter::Precision::Center);

    hook_painter->promptForName = [hook_name_dialog,hook_painter](){

        hook_name_dialog->on_confirm.clear();
        hook_name_dialog->on_confirm.subscribe([hook_painter](const std::string &name) {
            hook_painter->confirmPlacement(name);
        });

        if (!hook_name_dialog->shown) {
            hook_name_dialog->show();
            hook_name_dialog->setFocused(true);
        }
    };

    hook_painter->on_region_placed.subscribe([this](const std::string &name, Vector2Int TL, Vector2Int dimensions){
        state_machine->common->blueprint->hooks.addRegion(name,new HookRegion(TL,dimensions));
    });

    hook_painter->on_point_placed.subscribe([this](const std::string &name, Vector2Int position){
        state_machine->common->blueprint->hooks.addPoint(name,new HookPoint(position));
    });

    addedActiveEntities.push_back(hook_painter);

    auto region_deleter = new HookDeleter::RegionDeleter(&state_machine->common->blueprint->hooks,false);
    addedActiveEntities.push_back(region_deleter);
    auto point_deleter = new HookDeleter::PointDeleter(&state_machine->common->blueprint->hooks,false);
    addedActiveEntities.push_back(point_deleter);

    auto action_list = new GUIList(Anchor::TL,{100,0},160,GUI_Fill,std::vector<std::string>{
        "Snap Center",
        "Snap Intersection",
        "Snap Off",
        "Paint Region",
        "Paint Point",
        "Paint Off",
        "Delete Regions",
        "Delete Points"
    },
        [hook_painter,region_deleter,point_deleter](auto option) {
            if (option == "Snap Center") {
                hook_painter->setPrecision(HookPainter::Precision::Center);
            }
            else if (option == "Snap Intersection"){
                hook_painter->setPrecision(HookPainter::Precision::Intersection);
            }
            else if (option == "Snap Off"){
                hook_painter->setPrecision(HookPainter::Precision::Free);
            }
            else if (option == "Paint Region"){
                hook_painter->setMode(HookPainter::Mode::Regions);
                region_deleter->disable();
                point_deleter->disable();
            }
            else if (option == "Paint Point"){
                hook_painter->setMode(HookPainter::Mode::Point);
                region_deleter->disable();
                point_deleter->disable();
            }
            else if (option == "Paint Off"){
                hook_painter->setMode(HookPainter::Mode::Off);
            }
            else if (option == "Delete Regions") {
                hook_painter->setMode(HookPainter::Mode::Off);
                region_deleter->enable();
                point_deleter->disable();
            }
            else if (option == "Delete Points") {
                hook_painter->setMode(HookPainter::Mode::Off);
                region_deleter->disable();
                point_deleter->enable();
            }
        }
    );

    addedEditorGUIElements.push_back(action_list);

    auto appearance = new HookAppearance(&state_machine->common->blueprint->hooks);

    addedActiveEntities.push_back(appearance);

    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}

void ShipEditorModes::ModeHookPainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
