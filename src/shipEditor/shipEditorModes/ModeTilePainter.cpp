//
// Created by timon on 11/24/25.
//

#include "ModeTilePainter.h"

#include "../TilePainter/BlueprintTilePainter.h"
#include "userInterface/GUICheckbox.h"
#include "userInterface/GUILabel.h"
#include "userInterface/GUIList.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ModeTilePainter::enter() {

    addedActiveEntities = {};
    addedEditorGUIElements = {};

    auto common = state_machine->common;

    auto blueprint = common->blueprint;
    auto grid = common->grid;


    auto tile_painter = new BlueprintTilePainter(blueprint, grid, Tile::Wall,false);
    addedActiveEntities.push_back(tile_painter);
    

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
        Vector2Int(100, 0),
        100,
        GUI_Fill,
        tileOptions,
        [tile_painter](const std::string& option) {
            tile_painter->setTileToPaint(Tiles::tileFromName(option));
        },
        true
    );
    addedEditorGUIElements.push_back(tilesList);




    auto fill_label = new GUILabel(Anchor::TL,{200,0},50,25,"Fill",{255,255,255},fonts["sm"]);
    addedEditorGUIElements.push_back(fill_label);

    auto fillCheckbox = new GUICheckbox(
        Anchor::TL,
        {250,0},
        [tile_painter](bool checkboxValue){
            tile_painter->fill = checkboxValue;
        },
        tile_painter->fill
    );
    addedEditorGUIElements.push_back(fillCheckbox);




    addActiveEntities(addedActiveEntities);
    addGUIElements(addedEditorGUIElements);
}

void ShipEditorModes::ModeTilePainter::leave() {
    removeActiveEntities(addedActiveEntities);
    removeGUIElements(addedEditorGUIElements);
}
