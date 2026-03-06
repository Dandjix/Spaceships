#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <format>
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>

#include "scripts/IValuePrompt.h"
#include "userInterface/fonts.h"
#include "userInterface/GUIRect.h"
//
// Created by timon on 3/2/26.
//

namespace GUI::Prompts {
    class Select : public GUIRect, public virtual IValuePrompt<std::string> {
    protected:
        std::vector<std::string> options;
        int index;
        bool unfolded;

        static constexpr int OPTION_HEIGHT = 40;

    private:
        static void renderOption
        (SDL_Renderer *renderer, const std::string &option, int x_position, int y_start, Vector2Int dimensions,
         int *option_height = nullptr,
         SDL_Color text_color = {255, 255, 255, 255}, SDL_Color bg_color = {125, 125, 125, 255}) {
            SDL_FRect optionRect;
            SDL_RenderFillRect(renderer, &optionRect);

            SDL_Surface *surface = TTF_RenderText_Solid(fonts["sm"], option.c_str(), option.length(), text_color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (option_height != nullptr)
                *option_height = surface->h;

            optionRect.w = static_cast<float>(surface->w);
            optionRect.h = static_cast<float>(surface->h);
            optionRect.x = x_position + (dimensions.x - optionRect.w) / 2; // center horizontally
            optionRect.y = y_start; // start from start

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
              options(std::move(options)), index(index), unfolded(unfolded) {
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
            int found_index = std::distance(options.begin(), found);
            setIndex(found_index);
        }

        [[nodiscard]] bool inputIsValid() const override {
            return index >= 0 && index < options.size();
        }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
            auto sdl_f_rect = SDL_FRect(screenPosition.x, screenPosition.y, width, height);

            SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
            SDL_RenderFillRect(renderer, &sdl_f_rect);

            if (!unfolded) {
                if (inputIsValid())
                    renderOption(
                        renderer,
                        getValue(),
                        screenPosition.x,
                        screenPosition.y,
                        dimensions,
                        nullptr,
                        {255, 255, 255, 255},
                        {200, 200, 200, 255}
                    );
            } else {
                if (inputIsValid())
                    renderOption(
                        renderer,
                        getValue(),
                        screenPosition.x,
                        screenPosition.y,
                        dimensions,
                        nullptr,
                        {255, 255, 255, 255},
                        {200, 200, 200, 255}
                    );

                int y = OPTION_H;
                for (const std::string &option: options) {
                    int option_height;
                    renderOption(renderer, option, screenPosition.x, screenPosition.y + y, dimensions, &option_height);
                    y += option_height;
                }
            }
        }

        [[nodiscard]] GUIRect *asGUIRect() override { return this; }

        void handleEvent(const SDL_Event &event, const GameEvent::GameEventContext &context) override {
            if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN) return;

            float mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            if (unfolded) {
                // Vector2Int option_position = {screenPosition.x - dimensions.x/2 + screenPosition.y};
                // for (const std::string & option: options) {
                //
                // }
            } else {
                if (
                    (mouse_x >= screenPosition.x && mouse_x <= screenPosition.x + dimensions.x) &&
                    (mouse_y >= screenPosition.y && mouse_y <= screenPosition.y + dimensions.y)
                )
                    unfolded = true; //clicked on the folded select : unfold it
            }
        }
    };
}
