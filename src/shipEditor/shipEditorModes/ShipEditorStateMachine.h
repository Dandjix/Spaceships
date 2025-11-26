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
        virtual ~ShipEditorStateMachine() = default;

        CommonEditorObjects * common;

        std::vector<Entity * > * activeEntities;
        std::vector<Entity*> * activeEntitiesDeletionQueue;


        std::vector<GUIRect * > * editorGUIElements;
        std::vector<GUIRect*> * editorGUIElementsDeletionQueue;

        explicit ShipEditorStateMachine(
            CommonEditorObjects * common,
            std::vector<Entity * > * active,
            std::vector<Entity * > * activeEntitiesDeletionQueue,
            std::vector<GUIRect * > * GUIElements,
            std::vector<GUIRect * > * editorGUIElementsDeletionQueue,
            Mode initial)
        :
        common(common),
        current_mode_label(initial),
        activeEntities(active),
        activeEntitiesDeletionQueue(activeEntitiesDeletionQueue),
        editorGUIElements(GUIElements),
        editorGUIElementsDeletionQueue(editorGUIElementsDeletionQueue)
        {
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

        }

        Mode getMode() {
            return current_mode_label;
        }
    };
}
