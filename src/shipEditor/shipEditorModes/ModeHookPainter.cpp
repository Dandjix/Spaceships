//
// Created by timon on 11/24/25.
//

#include "ModeHookPainter.h"

#include "shipEditor/HookPainter.h"
#include "userInterface/GUICheckbox.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ModeHookPainter::enter() {
    auto * hook_painter = new HookPainter::Painter(
    state_machine->common.blueprint,
    HookPainter::Off,
    HookPainter::Intersection,
    []() {
    return "Point_1";
});
    auto hookCheckbox = new GUICheckbox(
    Anchor::TR,
    {250,100},
    [&hook_painter](bool checkboxValue){
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
    editorGUIElements.push_back(hookCheckbox);

    hook_painter->on_point_placed.subscribe([](std::string name, Vector2Int position) {
        std::cout << "point " << name << "has been placed at " << position << " (" << Vectors::toVector2Float(position).scaleToScreenPosition() / Tiles::tileSizePx << ")" << std::endl;
    });

    hook_painter->on_region_placed.subscribe([](std::string name, Vector2Int start, Vector2Int end) {
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
    activeEntities.push_back(hook_painter);
}

void ShipEditorModes::ModeHookPainter::leave() {
    removeActiveEntities(activeEntities);
    removeGUIElements(editorGUIElements);

    for (auto e: activeEntities) {
        delete e;
    }
    for (auto e: editorGUIElements) {
        delete e;
    }

    activeEntities = {};
    editorGUIElements = {};
}
