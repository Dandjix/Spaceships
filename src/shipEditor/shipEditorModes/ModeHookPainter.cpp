//
// Created by timon on 11/24/25.
//

#include "ModeHookPainter.h"

#include "../../userInterface/elements/GUI/GUICheckbox.h"
#include "ShipEditorStateMachine.h"
#include "shipEditor/HookPainter/HookAppearance.h"
#include "shipEditor/HookPainter/PainterStates/Painter.h"
#include "spaceships/Hooks/HookRegion.h"
#include "spaceships/Hooks/HookPoint.h"
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
        hook_name_dialog->show();
        hook_name_dialog->setFocused(true);
    };

    hook_painter->on_region_placed.subscribe([this](const std::string &name, Vector2Int TL, Vector2Int dimensions){
        state_machine->common->blueprint->hooks.addRegion(name,new HookRegion(TL,dimensions));
    });

    hook_painter->on_point_placed.subscribe([this](const std::string &name, Vector2Int position){
        state_machine->common->blueprint->hooks.addPoint(name,new HookPoint(position));
    });

    addedActiveEntities.push_back(hook_painter);
    auto hookCheckbox = new GUICheckbox(
    Anchor::TR,
    {250,100},
    [hook_painter](bool checkboxValue){
        switch (auto s = hook_painter->getMode()) {
                case HookPainter::Mode::Off:
                    hook_painter->setMode(HookPainter::Mode::Regions);
                    break;
            case HookPainter::Mode::Regions:
                    hook_painter->setMode(HookPainter::Mode::Point);
                    break;
            case HookPainter::Mode::Point:
                    hook_painter->setMode(HookPainter::Mode::Off);
                    break;
            }
        },
        false
    );
    addedEditorGUIElements.push_back(hookCheckbox);

    auto appearance = new HookAppearance(&state_machine->common->blueprint->hooks);

    addedActiveEntities.push_back(appearance);

    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}

void ShipEditorModes::ModeHookPainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
