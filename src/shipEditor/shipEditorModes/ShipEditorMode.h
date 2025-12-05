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
    protected:

        void addActiveEntities(const std::vector<Entity * > &to_add) const;

        void removeActiveEntities(const std::vector<Entity * > &to_remove) const;


        void addGUIElements(const std::vector<GUIRect * > &to_add) const;

        void removeGUIElements(const std::vector<GUIRect * > &to_remove) const;

    public:
        virtual ~ShipEditorMode() = default;

        ShipEditorStateMachine * state_machine;

        explicit ShipEditorMode(ShipEditorStateMachine * state_machine) : state_machine(state_machine){}

        virtual void enter() = 0;
        virtual void leave() = 0;
    };
}
