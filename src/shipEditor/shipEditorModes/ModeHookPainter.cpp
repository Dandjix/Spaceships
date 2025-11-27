//
// Created by timon on 11/24/25.
//

#include "ModeHookPainter.h"

#include "../HookPainter/HookPainter.h"
#include "userInterface/GUICheckbox.h"
#include "ShipEditorStateMachine.h"
#include "shipEditor/HookPainter/HookAppearance.h"
#include "spaceships/Hooks/HookRegion.h"
#include "spaceships/Hooks/HookPoint.h"

void ShipEditorModes::ModeHookPainter::enter() {

    addedActiveEntities = {};
    addedEditorGUIElements = {};

    auto * hook_painter = new HookPainter::Painter(
        HookPainter::Off,
        HookPainter::Intersection,
        []() {
        return "Point_1";
    });

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
        switch (auto s = hook_painter->getState()) {
                case HookPainter::Off:
                    hook_painter->setState(HookPainter::Regions);
                    break;
                case HookPainter::Regions:
                    hook_painter->setState(HookPainter::Point);
                    break;
                case HookPainter::Point:
                    hook_painter->setState(HookPainter::Off);
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
