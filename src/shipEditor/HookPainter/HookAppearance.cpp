//
// Created by timon on 11/27/25.
//

#include "HookAppearance.h"

#include "spaceships/Hooks/HookPoint.h"
#include "spaceships/Hooks/HookRegion.h"

void HookAppearance::render(SDL_Renderer *renderer, const RenderingContext &context) {

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for (const auto r: hooks->regions | std::views::values) {

        std::vector corners = {
            context.camera_info.worldToScreenPoint(r->TL),
            context.camera_info.worldToScreenPoint({r->TL.x, r->TL.y + r->dimensions.y}),
            context.camera_info.worldToScreenPoint(r->TL + r->dimensions),
            context.camera_info.worldToScreenPoint({r->TL.x + r->dimensions.x,r->TL.y})
        };


        for (int i = 0; i < corners.size(); ++i) {
            int next_i = (i+1)%corners.size();
            SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
        }
    }

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    for (const auto p: hooks->points | std::views::values) {
        std::vector corners = {
            context.camera_info.worldToScreenPoint(p->position + Vector2Int(32,32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(p->position + Vector2Int(-32,32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(p->position + Vector2Int(-32,-32).scaleToWorldPosition()),
            context.camera_info.worldToScreenPoint(p->position + Vector2Int(32,-32).scaleToWorldPosition())
        };


        for (int i = 0; i < corners.size(); ++i) {
            int next_i = (i+1)%corners.size();
            SDL_RenderLine(renderer,corners[i].x,corners[i].y,corners[next_i].x,corners[next_i].y);
        }
    }
}
