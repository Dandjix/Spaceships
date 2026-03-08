//
// Created by timon on 11/24/25.
//

#include "ModeTilePainter.h"

#include "../TilePainter/BlueprintTilePainter.h"
#include "../../userInterface/elements/GUI/GUICheckbox.h"
#include "../../userInterface/elements/GUI/GUILabel.h"
#include "../../userInterface/elements/GUI/GUIList.h"
#include "ShipEditorStateMachine.h"

void ShipEditorModes::ModeTilePainter::createEntitiesAndElements() {

    auto common = state_machine->common;

    auto blueprint = common->blueprint;
    auto grid = common->grid;


    auto tile_painter = new BlueprintTilePainter(blueprint, grid, Tile::Wall,false);
    added_entities.push_back(tile_painter);
    

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
    added_ui_elements.push_back(tilesList);




    auto fill_label = new GUILabel(Anchor::TL,{200,0},50,25,"Fill",{255,255,255},fonts["sm"]);
    added_ui_elements.push_back(fill_label);

    auto fillCheckbox = new GUICheckbox(
        Anchor::TL,
        {250,0},
        [tile_painter](bool checkboxValue){
            tile_painter->fill = checkboxValue;
        },
        tile_painter->fill
    );
    added_ui_elements.push_back(fillCheckbox);
}
