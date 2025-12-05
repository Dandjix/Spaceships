//
// Created by timon on 12/5/25.
//

#pragma once
#include "GUILabel.h"

namespace GUI {
    class FramedLabel final : public GUILabel {
    protected:
        float padding;
        SDL_Color inner_color;
        SDL_Color outer_color;

    public:
        FramedLabel(Anchor anchor, const Vector2Int &offset, int width, int height, const std::string &labelText,
                    const SDL_Color &labelColor, TTF_Font *labelFont, float margin = 5.0f,
                    SDL_Color inner_color = {0, 0, 0, 255}, SDL_Color outer_color = {255, 255, 255, 255},
                    QueueOrder::Value queue_order = QueueOrder::MIDDLE)
            : GUILabel(anchor, offset, width, height, labelText, labelColor, labelFont, queue_order), padding(margin),
              inner_color(inner_color), outer_color(outer_color) {
        }


        void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override {
            SDL_FRect outer = {
                static_cast<float>(screenPosition.x),
                static_cast<float>(screenPosition.y),
                static_cast<float>(width != GUI_Fill ? width : context.camera_info.screenDimensions.x),
                static_cast<float>(height != GUI_Fill ? height : context.camera_info.screenDimensions.y)
            };

            SDL_FRect inner = {
                static_cast<float>(screenPosition.x) + padding,
                static_cast<float>(screenPosition.y) + padding,
                static_cast<float>(width != GUI_Fill ? width : context.camera_info.screenDimensions.x) - padding * 2,
                static_cast<float>(height != GUI_Fill ? height : context.camera_info.screenDimensions.y) - padding * 2
            };

            SDL_SetRenderDrawColor(renderer, outer_color.r, outer_color.g, outer_color.b, outer_color.a);
            SDL_RenderFillRect(renderer, &outer);
            SDL_SetRenderDrawColor(renderer, inner_color.r, inner_color.g, inner_color.b, inner_color.a);
            SDL_RenderFillRect(renderer, &inner);

            GUILabel::render(renderer, context);
        }
    };
}
