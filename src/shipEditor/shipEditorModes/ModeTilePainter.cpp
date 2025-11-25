//
// Created by timon on 11/24/25.
//

#include "ModeTilePainter.h"

#include "shipEditor/BlueprintTilePainter.h"
#include "userInterface/GUICheckbox.h"
#include "userInterface/GUILabel.h"
#include "userInterface/GUIList.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ModeTilePainter::enter() {
    auto common = state_machine->common;

    auto blueprint = common.blueprint;
    auto grid = common.grid;


    auto tile_painter = new BlueprintTilePainter(blueprint, grid, Tile::Wall,false);
    activeEntities.push_back(tile_painter);


    std::vector<std::string>tileOptions =
    {
        "Void",
        "Wall",
        "DoorH",
        "DoorV",
        "Floor"
    };
    auto tilesList = new GUIList(
        Anchor::TL,
        Vector2Int(0, 0),
        100,
        GUI_Fill,
        tileOptions,
        [tile_painter](const std::string& option) {
            tile_painter->setTileToPaint(Tiles::tileFromName(option));
        },
        true
    );
    editorGUIElements.push_back(tilesList);



    editorGUIElements.push_back(new GUILabel(Anchor::TL,{200,0},50,25,"Fill",{255,255,255},fonts["sm"]));

    auto fillCheckbox = new GUICheckbox(
        Anchor::TL,
        {250,0},
        [tile_painter](bool checkboxValue){
            tile_painter->fill = checkboxValue;
        },
        tile_painter->fill
    );
    editorGUIElements.push_back(fillCheckbox);


    addActiveEntities(activeEntities);
    addGUIElements(editorGUIElements);

}

void ShipEditorModes::ModeTilePainter::leave() {
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
