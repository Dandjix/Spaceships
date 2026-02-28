

#include "scripts/GUIStringPrompt.h"
#include "scripts/IGUIValuePrompt.h"

class GUITextPrompt : public GUIStringPrompt , public virtual IGUIValuePrompt<std::string> {
public:
    GUITextPrompt(Anchor anchor, const Vector2Int &offset, int width, int height, SDL_Window *window, bool shown,
        const std::string &value = "", const std::string &placeholder = "", bool element_is_focused = true)
        : GUIStringPrompt(anchor, offset, width, height, window, shown, value, placeholder, element_is_focused) {
    }

    std::string getValue() override {return value;}

    void setValue(std::string new_value) override {value = new_value;}

    bool inputIsValid() override {return true;} // string validation is done here
};
