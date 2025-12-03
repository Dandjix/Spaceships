//
// Created by timon on 12/3/25.
//

#pragma once
#include <vector>

#include "game/PauseManager.h"
#include "userInterface/GUIRect.h"
#include "userInterface/elements/GUI/GUIList.h"
#include <algorithm>

class PauseMenu {
private:
    std::vector<GUIRect *> pause_menu_elements;
    std::vector<GUIRect *> *gui_elements;

    std::vector<std::string> get_option_keys(std::map<std::string, std::function<void()> > m);

public:
    std::map<std::string, std::function<void()> > actions;

    explicit PauseMenu(
        PauseManager *pause_manager,
        const std::map<std::string, std::function<void()> >& actions,
        std::vector<GUIRect *> * gui_elements
    );

    void show();

    void hide();
};
