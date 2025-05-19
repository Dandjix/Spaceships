#pragma once

#include "GUIRect.h"
#include "Update.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>

class GUIList : public GUIRect
{
private:
    SDL_FRect buttonRect(int i) const;

    int outerPadding = 10;
    int marginY = 5; //margin between buttons
    int buttonHeight = 25;
    int buttonWidth = 0;
    bool keyboardNavigation;

protected:
    std::vector <std::string> options;
    std::function<void(std::string option)> onSelect;
    int highlighted_index = 0;
    int selected_index = -1;
public:
    std::string getSelectedOption();

    GUIList(
        Anchor anchor,
        Vector2Int offset,
        int width,
        int height,
        std::vector<std::string> options,
        std::function<void(std::string)> onSelect,
        bool keyboardNavigation = false
    )
        : GUIRect(anchor, offset, width, height), options(options), onSelect(onSelect), keyboardNavigation(keyboardNavigation){
    }

    void handleEvent(const SDL_Event & event) override;

    void update(const GUI_UpdateContext& context) override;

    void render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const override;
};
