//
// Created by timon on 11/24/25.
//

#pragma once
#include "userInterface/GUIRect.h"
#include <vector>
#include <unordered_set>
#include <algorithm>


namespace ShipEditorModes {
    class ShipEditorStateMachine;

    class ShipEditorMode {
    public:
        ShipEditorStateMachine * state_machine;
    protected:
        std::vector<Entity *> added_entities;
        std::vector<GUIRect *> added_ui_elements;

        void addActiveEntities(const std::vector<Entity * > &to_add) const;

        void fakeKillActiveEntities(const std::vector<Entity * > &to_remove) const;


        void addGUIElements(const std::vector<GUIRect * > &to_add) const;

        void killGUIElements(const std::vector<GUIRect * > &to_remove) const;

    public:
        virtual ~ShipEditorMode() = default;


        explicit ShipEditorMode(ShipEditorStateMachine * state_machine) : state_machine(state_machine){}

        void enter();
        void leave();

        virtual void createEntitiesAndElements() = 0;
        virtual void afterLeave() {};
    };
}
