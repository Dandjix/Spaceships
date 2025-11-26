//
// Created by timon on 11/24/25.
//

#include "ModeHookPainter.h"

#include "shipEditor/HookPainter.h"
#include "userInterface/GUICheckbox.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ModeHookPainter::enter() {

    addedActiveEntities = {};
    addedEditorGUIElements = {};

    auto * hook_painter = new HookPainter::Painter(
        state_machine->common->blueprint,
        HookPainter::Off,
        HookPainter::Intersection,
        []() {
        return "Point_1";
    });
    addedActiveEntities.push_back(hook_painter);
    auto hookCheckbox = new GUICheckbox(
    Anchor::TR,
    {250,100},
    [hook_painter](bool checkboxValue){
        switch (auto s = hook_painter->getState()) {
                case HookPainter::Off:
                    hook_painter->setState(HookPainter::Regions);
                    std::cout << "set to regions" << std::endl;
                    break;
                case HookPainter::Regions:
                    hook_painter->setState(HookPainter::Point);
                    std::cout << "set to point" << std::endl;
                    break;
                case HookPainter::Point:
                    hook_painter->setState(HookPainter::Off);
                    std::cout << "set to off" << std::endl;
                    break;
            }
        },
        false
    );
    addedEditorGUIElements.push_back(hookCheckbox);

    hook_painter->on_point_placed.subscribe([](const std::string &name, Vector2Int position) {
        std::cout << "point " << name << "has been placed at " << position << " (" << Vectors::toVector2Float(position).scaleToScreenPosition() / Tiles::tileSizePx << ")" << std::endl;
    });

    hook_painter->on_region_placed.subscribe([](const std::string &name, Vector2Int start, Vector2Int end) {
        std::cout <<
        "region " <<
        name <<
        "has been placed at : " <<
        start <<
        ", " << end <<
        " (" <<
        Vectors::toVector2Float(start).scaleToScreenPosition() / Tiles::tileSizePx <<
        ", " << Vectors::toVector2Float(end).scaleToScreenPosition() / Tiles::tileSizePx <<
        ")" << std::endl;
    });

    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}

void ShipEditorModes::ModeHookPainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
