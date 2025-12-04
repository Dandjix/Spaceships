//
// Created by timon on 12/3/25.
//

#include "PauseMenu.h"

#include <format>

#include "userInterface/elements/GUI/GUIColoredRect.h"

std::vector<std::string> PauseMenu::get_option_keys(std::vector<std::pair<std::string, std::function<void()>>> options) {
    std::vector<std::string> keys;
    for (auto &key: options | std::views::keys) {
        keys.push_back(key);
    }
    return keys;
}

void PauseMenu::execute_action(const std::string &value) {
    for (const auto& [key,action]: actions) {
        if (key == value) {
            action();
            return;
        }
    }
    throw std::invalid_argument(std::format("key {} not found in action vector",value));
}

PauseMenu::PauseMenu(PauseManager *pause_manager, const std::vector<std::pair<std::string, std::function<void()>>> &actions,
                     std::vector<GUIRect *> *gui_elements): actions(actions), gui_elements(gui_elements) {
    pause_manager->on_paused_change.subscribe([this](bool paused) {
        if (paused)
            show();
        else
            hide();
    });

    pause_menu_elements
            =
            std::vector{
                static_cast<GUIRect *>(
                    new GUIList(Anchor::Center, {0, 0}, 500, 600, get_option_keys(actions),
                                [this](const std::string &option) {
                                    execute_action(option);
                                },true)),
                static_cast<GUIRect *>(new GUIColoredRect(Anchor::Center,{0,0},GUI_Fill,GUI_Fill,{0,0,0,150},QueueOrder::MIDDLE + 50) )
            };
}

void PauseMenu::show() {
    for (auto to_add: pause_menu_elements) {
        gui_elements->push_back(to_add);
    }
}

void PauseMenu::hide() {
    for (auto to_remove: pause_menu_elements) {
        gui_elements->erase(std::remove(gui_elements->begin(), gui_elements->end(), to_remove), gui_elements->end());
    }
}
