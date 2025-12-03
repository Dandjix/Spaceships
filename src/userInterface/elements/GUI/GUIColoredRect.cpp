//
// Created by timon on 12/3/25.
//

#include "GUIColoredRect.h"

GUIColoredRect::GUIColoredRect(Anchor anchor, const Vector2Int &offset, int width, int height, SDL_Color color,
    unsigned short queueOrder, bool capture_mouse_clicks): GUIRect(anchor, offset, width, height),color(color), queueOrder(queueOrder), capture_mouse_clicks(capture_mouse_clicks) {
}

bool GUIColoredRect::is_inside(Vector2Float position) {
    if (!capture_mouse_clicks)
        return false;
    return GUIRect::is_inside(position);
}

void GUIColoredRect::render(SDL_Renderer *renderer, const GUI_RenderingContext &context) const {
    SDL_FRect rect = {
        static_cast<float>(screenPosition.x),
        static_cast<float>(screenPosition.y),
        static_cast<float>(dimensions.x),
        static_cast<float>(dimensions.y)
    };
    SDL_BlendMode mode;
    SDL_GetRenderDrawBlendMode(renderer,&mode);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);

    SDL_SetRenderDrawBlendMode(renderer,mode);
}

QueueOrder::Value GUIColoredRect::getQueueOrder() {
    return queueOrder;
}
