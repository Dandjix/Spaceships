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

        template <typename T>
        void removeItems(std::vector<T>* active,
                         const std::vector<T>& to_remove)
        {
            std::unordered_set<T> remove_set(to_remove.begin(), to_remove.end());

            active->erase(
                std::remove_if(active->begin(), active->end(),
                    [&](const T& item) {
                        return remove_set.contains(item);
                    }),
                active->end()
            );
        }

        template <typename T>
        void addItems(std::vector<T>* active,
            const std::vector<T>& to_add) {
            for (auto e: to_add) {
                active->push_back(e);
            }
        }

        void addActiveEntities(const std::vector<Entity * > &to_add);

        void removeActiveEntities(const std::vector<Entity * > &to_remove);


        void addGUIElements(const std::vector<GUIRect * > &to_add);

        void removeGUIElements(const std::vector<GUIRect * > &to_remove);

    public:
        virtual ~ShipEditorMode() = default;

        ShipEditorStateMachine * state_machine;

        explicit ShipEditorMode(ShipEditorStateMachine * state_machine) : state_machine(state_machine){}

        virtual void enter() = 0;
        virtual void leave() = 0;
    };
}
