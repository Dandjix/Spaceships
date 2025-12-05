//
// Created by timon on 11/15/25.
//

#pragma once
#include <utility>

#include "userInterface/fonts.h"
#include "userInterface/GUIRect.h"

class GUILabel : public GUIRect {
public:
    TTF_Font *font;
    std::string text;
    SDL_Color color;
    QueueOrder::Value queue_order;

    GUILabel(Anchor anchor, Vector2Int offset, int width, int height, std::string labelText, SDL_Color labelColor,
             TTF_Font *labelFont, QueueOrder::Value queue_order = QueueOrder::MIDDLE) : GUIRect(anchor, offset, width, height),
                                                                   font(labelFont), text(std::move(labelText)),
                                                                   color(labelColor),
                                                                   queue_order(queue_order) {
    }

    void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;

    QueueOrder::Value getQueueOrder() override {
        return queue_order;
    }
};
