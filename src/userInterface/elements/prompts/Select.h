#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <format>
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>

#include "gameEvent/GameEvent.h"
#include "scripts/IValuePrompt.h"
#include "userInterface/fonts.h"
#include "userInterface/GUIRect.h"
//
// Created by timon on 3/2/26.
//

namespace GUI::Prompts {
    class Select : public GUIRect, public virtual IValuePrompt<std::string> {
    public:
        Event<std::string> on_option_selected;

    protected:
        std::vector<std::string> options;
        int index;
        bool unfolded;
        int highlighted_index;

        static constexpr int OPTION_HEIGHT = 40;

    private:
        static std::vector<std::pair<Vector2Int, Vector2Int> > getUnfoldedOptionsLayout(
            Vector2Int select_position, Vector2Int select_dimensions, const std::vector<std::string> &options) {
            std::vector<std::pair<Vector2Int, Vector2Int> > layouts = {};
            layouts.reserve(options.size());
            Vector2Int position = {select_position.x, select_position.y + select_dimensions.y};
            for (const std::string &option: options) {
                layouts.push_back({position, select_dimensions});
                position.y += OPTION_HEIGHT;
            }
            return layouts;
        }

        static void renderOption
        (SDL_Renderer *renderer,
         const std::string &option,
         Vector2Int position,
         Vector2Int dimensions,
         SDL_Color text_color = {255, 255, 255, 255},
         SDL_Color bg_color = {125, 125, 125, 255}
        ) {
            SDL_FRect bgRectRect;
            SDL_FRect optionRect;

            SDL_Surface *surface = TTF_RenderText_Solid(fonts["sm"], option.c_str(), option.length(), text_color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            bgRectRect.w = dimensions.x;
            bgRectRect.h = dimensions.y;
            bgRectRect.x = position.x;
            bgRectRect.y = position.y;

            optionRect.w = static_cast<float>(surface->w);
            optionRect.h = static_cast<float>(surface->h);
            optionRect.x = position.x;
            optionRect.y = position.y;

            SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
            SDL_RenderFillRect(renderer, &bgRectRect);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderRect(renderer, &bgRectRect);
            SDL_RenderTexture(renderer, texture, nullptr, &optionRect);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }

    public:
        Select(
            Anchor anchor,
            Vector2Int offset,
            int width,
            int height,
            std::vector<std::string> options,
            int index = -1,
            bool unfolded = false
        )
            : GUIRect(anchor, offset, width, height),
              options(std::move(options)), index(index), unfolded(unfolded), highlighted_index(-1) {
            if (this->options.empty()) throw std::invalid_argument("options must not be empty");
        }

        [[nodiscard]] std::string getValue() const override {
            return options.at(index);
        }

        void setIndex(int value) {
            index = std::clamp(value, -1, static_cast<int>(options.size() - 1));
        }

        void setValue(std::string value) override {
            auto found = std::ranges::find(options, value);
            if (found == options.end()) {
                throw std::invalid_argument(std::format(
                    "value : {} could not be found among select options ({} options)", value, options.size()));
            }
            int found_index = static_cast<int>(std::distance(options.begin(), found));
            setIndex(found_index);
        }

        [[nodiscard]] bool inputIsValid() const override {
            return index >= 0 && index < options.size();
        }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
            auto sdl_f_rect = SDL_FRect(screenPosition.x, screenPosition.y, width, height);

            SDL_Color bg_color = unfolded ? SDL_Color{200, 200, 200, 255} : SDL_Color{255, 100, 100, 255};
            SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
            SDL_RenderFillRect(renderer, &sdl_f_rect);


            if (!unfolded) {
                if (inputIsValid())
                    renderOption(
                        renderer,
                        getValue(),
                        screenPosition,
                        dimensions,
                        {255, 255, 255, 255},
                        bg_color
                    );
            } else {
                if (inputIsValid())
                    renderOption(
                        renderer,
                        getValue(),
                        screenPosition,
                        dimensions,
                        {255, 255, 255, 255},
                        bg_color
                    );

                auto layout = getUnfoldedOptionsLayout(screenPosition, dimensions, options);
                for (int i = 0; i < options.size(); i++) {
                    const auto &[position,dimensions] = layout.at(i);
                    const std::string &option = options.at(i);

                    SDL_Color option_text_color = {255, 255, 255, 255};
                    SDL_Color option_bg_color = (highlighted_index == i)
                                                    ? SDL_Color{125, 125, 125, 255}
                                                    : SDL_Color{160, 160, 160, 255};

                    renderOption(renderer, option, position, dimensions, option_text_color, option_bg_color);
                }
            }
        }

        [[nodiscard]] GUIRect *asGUIRect() override { return this; }

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override {
            if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN) return;

            if (context.element_under_mouse != this) {
                unfolded = false;
                return;
            }

            float mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);


            if (unfolded) {
                auto layout = getUnfoldedOptionsLayout(screenPosition, dimensions, options);
                for (int i = 0; i < options.size(); i++) {
                    const auto &[position,dimensions] = layout.at(i);
                    const std::string &option = options.at(i);
                    if (
                        (mouse_x >= position.x && mouse_x <= position.x + dimensions.x) &&
                        (mouse_y >= position.y && mouse_y <= position.y + dimensions.y)
                    ) {
                        on_option_selected.emit(option);
                        setValue(option);
                        unfolded = false; //when we select an option, we fold the dialog
                        return;
                    }
                }
            } else {
                if (
                    (mouse_x >= screenPosition.x && mouse_x <= screenPosition.x + dimensions.x) &&
                    (mouse_y >= screenPosition.y && mouse_y <= screenPosition.y + dimensions.y)
                )
                    unfolded = true; //clicked on the folded select : unfold it
                std::cout << "unfolded select" << std::endl;
            }
        }

        QueueOrder::Value getQueueOrder() override {
            return QueueOrder::LAST - 500;
        }

        bool is_inside(Vector2Float position) override {
            if (!unfolded)
                return GUIRect::is_inside(position);

            auto layout = getUnfoldedOptionsLayout(screenPosition, dimensions, options);

            const auto &[first_option_position,first_option_dimensions] = layout.at(0);
            const auto &[last_option_position,last_option_dimensions] = layout.at(layout.size() - 1);

            Vector2Int top_left_options = {first_option_position.x, first_option_position.y};
            Vector2Int bottom_right_options = {
                last_option_position.x + last_option_dimensions.x, last_option_position.y + last_option_dimensions.y
            };

            return GUIRect::is_inside(position) ||
                   ((position.x >= top_left_options.x && position.x <= bottom_right_options.x)
                    // if we are over the options, we are over the element
                    && (position.y >= top_left_options.y && position.y <= bottom_right_options.y));
        }

        void update(const GUI_UpdateContext &context) override {
            GUIRect::update(context);

            if (context.element_under_mouse != this) {
                highlighted_index = -1;
                return;
            }

            auto layout = getUnfoldedOptionsLayout(screenPosition, dimensions, options);

            float mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);


            for (int i = 0; i < layout.size(); i++) {
                Vector2Int option_position = layout.at(i).first;
                Vector2Int option_dimensions = layout.at(i).second;

                if (mouse_x >= option_position.x && mouse_x <= option_position.x + option_dimensions.x
                    && mouse_y >= option_position.y && mouse_y <= option_position.y + option_dimensions.y) {
                    highlighted_index = i;
                    return;
                }
            }
            highlighted_index = -1;
        }
    };
}
