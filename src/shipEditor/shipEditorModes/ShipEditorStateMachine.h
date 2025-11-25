//
// Created by timon on 11/24/25.
//

#pragma once
#include "CommonEditorEntities.h"
#include "ModeHookPainter.h"
#include "ModeTilePainter.h"

class GUIRect;
class Entity;

namespace ShipEditorModes {
    class ShipEditorMode;

    enum Mode {
        TilePainter,
        HookPainter
    };

    class ShipEditorStateMachine {
    private:
        Mode current_mode_label;
        ShipEditorMode * current_mode;
        ModeTilePainter * tile_painter_mode;
        ModeHookPainter * hook_painter_mode;

    public:
        CommonEditorObjects common;
        std::vector<Entity * > * activeEntities;
        std::vector<GUIRect * > * GUIElements;

        explicit ShipEditorStateMachine(CommonEditorObjects common, std::vector<Entity * > * active, std::vector<GUIRect * > * GUIElements,Mode initial)
        :
        common(common), current_mode_label(initial), activeEntities(active),GUIElements(GUIElements) {
            tile_painter_mode = new ModeTilePainter(this);
            hook_painter_mode = new ModeHookPainter(this);
            switch (initial) {
                case TilePainter:
                    current_mode = tile_painter_mode;
                    break;
                case HookPainter:
                    current_mode = hook_painter_mode;
                    break;
            }
            current_mode->enter();
        }

        void setMode(Mode label) {
            if (label == current_mode_label)
                return ;

            current_mode->leave();
            std::cout << "left with success" << std::endl;
            //Assignments
            current_mode_label = label;
            switch (label) {
                case TilePainter:
                    current_mode = tile_painter_mode;
                    break;
                case HookPainter:
                    current_mode = hook_painter_mode;
                    break;
            }

            current_mode->enter();
            std::cout << "entered with success" << std::endl;

        }

        Mode getMode() {
            return current_mode_label;
        }
    };
}
