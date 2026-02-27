//
// Created by timon on 2/28/26.
//

#include "Interface.h"

#include "game/Rendering.h"

void EntityPlacement::Interface::render(SDL_Renderer *renderer, const RenderingContext &context) {
    //we render a bg rect

    float total_height = form_request.computeHeight();

    auto center = (context.camera_info.screenDimensions) / 2;
    Vector2Float dimensions = {1000.0f, total_height};
    auto rect = SDL_FRect(center.x - dimensions.x / 2, center.y - dimensions.y / 2, 0, 0);

    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderRect(renderer, &rect);
}
