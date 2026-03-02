

#include "scripts/GUIStringPrompt.h"
#include "scripts/IValuePrompt.h"
namespace GUI::Prompts {

class TextPrompt : public StringPrompt , public virtual IValuePrompt<std::string> {
public:
    TextPrompt(Anchor anchor, const Vector2Int &offset, int width, int height, SDL_Window *window, bool shown,
        const std::string &value = "", const std::string &placeholder = "", bool element_is_focused = true)
        : StringPrompt(anchor, offset, width, height, window, shown, value, placeholder, element_is_focused) {
    }

    [[nodiscard]] std::string getValue() const override {return value;}

    void setValue(std::string new_value) override {value = new_value;}

    [[nodiscard]] [[nodiscard]] bool inputIsValid() const override {return true;} // string validation is done here
};
}
