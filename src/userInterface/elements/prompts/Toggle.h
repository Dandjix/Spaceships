//
// Created by timon on 3/7/26.
//

#pragma once
#include "scripts/IValuePrompt.h"
#include "userInterface/elements/GUI/TextButton.h"

namespace GUI::Prompts {
    class Toggle : public TextButton, public virtual IValuePrompt<bool> {
    protected:
        std::string annotation;
        bool value;

        static std::string computeLabel(bool value) {
            return value ? "Yes" : "No";
        }

    public:
        Toggle(Anchor anchor,
               const Vector2Int &offset,
               int width,
               int height,
               bool value = false,
               std::string annotation = "")
            : TextButton(anchor,
                         offset,
                         width,
                         height,
                         computeLabel(value)),
              value(value), annotation(std::move(annotation)) {
            on_pressed.subscribe([this]() {
                this->value = !this->value;
                this->text = computeLabel(this->value);
            });
        }

        [[nodiscard]] bool inputIsValid() const override { return true; }

        [[nodiscard]] GUIRect *asGUIRect() override { return this; }

        [[nodiscard]] bool getValue() const override { return value; }

        void setValue(bool new_value) override { value = new_value; }

        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
            TextButton::render(renderer, context);

            {
                SDL_Surface *surface = TTF_RenderText_Solid(
                    fonts["sm"],
                    annotation.c_str(),
                    annotation.length(),
                    {255, 100, 100, 255}
                );
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

                SDL_FRect textRect;
                textRect.w = static_cast<float>(surface->w);
                textRect.h = static_cast<float>(surface->h);
                textRect.x = screenPosition.x;
                textRect.y = screenPosition.y;

                SDL_RenderTexture(renderer, texture, nullptr, &textRect);

                SDL_DestroyTexture(texture);
                SDL_DestroySurface(surface);
            }
        }
    };
}
