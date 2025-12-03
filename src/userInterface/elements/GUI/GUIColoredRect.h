//
// Created by timon on 12/3/25.
//

#pragma once
#include "userInterface/GUIRect.h"


class GUIColoredRect : public GUIRect{
private:
    unsigned short queueOrder;
    bool capture_mouse_clicks;
    SDL_Color color;
public:
    GUIColoredRect(Anchor anchor, const Vector2Int &offset, int width, int height,SDL_Color color, unsigned short queueOrder = QueueOrder::MIDDLE, bool capture_mouse_clicks = false);

    bool is_inside(Vector2Float position) override;

    void render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const override;

    QueueOrder::Value getQueueOrder() override;
};
