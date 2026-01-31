//
// Created by timon on 11/24/25.
//

#pragma once
#include "CommonEditorEntities.h"
#include "ModeAirlockPainter.h"
#include "ModeEntityPainter.h"
#include "ModeHookPainter.h"
#include "ModeTilePainter.h"
#include "ModeToggleableLinker.h"
#include "game/ElementContainer.h"

class GUIRect;
class Entity;

namespace ShipEditorModes {
    enum Mode {
        TilePainter,
        HookPainter,
        AirlockPainter,
        EntityPainter,
        ToggleableLinker
    };

    class ShipEditorStateMachine {
    private:
        Mode current_mode_label;
        ShipEditorMode *current_mode;
        ModeTilePainter *tile_painter_mode;
        ModeHookPainter *hook_painter_mode;
        ModeAirlockPainter *airlock_painter_mode;
        ModeEntityPainter *entity_painter_mode;
        ModeToggleableLinker *toggleable_linker_mode;

    public:
        SDL_Window *window;

        virtual ~ShipEditorStateMachine() = default;

        CommonEditorObjects *common;

        ElementContainer<Entity *> *activeEntities;
        ElementContainer<Entity *> *activeEntitiesDeletionQueue;

        ElementContainer<GUIRect *> *editorGUIElements;
        ElementContainer<GUIRect *> *editorGUIElementsDeletionQueue;

        explicit ShipEditorStateMachine(
            CommonEditorObjects *common,
            ElementContainer<Entity *> *active,
            ElementContainer<Entity *> *activeEntitiesDeletionQueue,
            ElementContainer<GUIRect *> *GUIElements,
            ElementContainer<GUIRect *> *editorGUIElementsDeletionQueue,
            Mode initial,
            SDL_Window *window)
            : common(common),
              current_mode_label(initial),
              activeEntities(active),
              activeEntitiesDeletionQueue(activeEntitiesDeletionQueue),
              editorGUIElements(GUIElements),
              editorGUIElementsDeletionQueue(editorGUIElementsDeletionQueue),
              window(window) {
            tile_painter_mode = new ModeTilePainter(this);
            hook_painter_mode = new ModeHookPainter(this);
            airlock_painter_mode = new ModeAirlockPainter(this);
            entity_painter_mode = new ModeEntityPainter(this);
            toggleable_linker_mode = new ModeToggleableLinker(this);

            switch (initial) {
                case TilePainter:
                    current_mode = tile_painter_mode;
                    break;
                case HookPainter:
                    current_mode = hook_painter_mode;
                    break;
                case AirlockPainter:
                    current_mode = airlock_painter_mode;
                    break;
                case EntityPainter:
                    current_mode = entity_painter_mode;
                    break;
                case ToggleableLinker:
                    current_mode = toggleable_linker_mode;
                    break;
            }
            current_mode->enter();
        }

        void setMode(Mode label) {
            if (label == current_mode_label)
                return;

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
                case AirlockPainter:
                    current_mode = airlock_painter_mode;
                    break;
                case EntityPainter:
                    current_mode = entity_painter_mode;
                    break;
                case ToggleableLinker:
                    current_mode = toggleable_linker_mode;
                    break;
            }

            current_mode->enter();
        }

        Mode getMode() {
            return current_mode_label;
        }
    };
}
