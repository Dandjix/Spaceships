//
// Created by timon on 11/14/25.
//

#include "GUICheckbox.h"

const int middle_rect_relative_size = 4;
const int inner_rect_relative_size = 4;

void GUICheckbox::handleEvent(const SDL_Event& event, const GameEvent::GameEventContext & context)
{
    if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        return;
    }

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);

    if (is_inside(Vector2Float(mouse_x,mouse_y)))
    {
        value = ! value;
        onValueChanged(value);
    }
}

void GUICheckbox::render(SDL_Renderer* renderer, const GUI_RenderingContext& context) const
{
    SDL_FRect bgRect{
        static_cast<float>(screenPosition.x),
        static_cast<float>(screenPosition.y),
        static_cast<float>(dimensions.x),
        static_cast<float>(dimensions.y)
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_FRect middleRect{
        static_cast<float>(screenPosition.x + 0.5f*middle_rect_relative_size),
        static_cast<float>(screenPosition.y + 0.5f*middle_rect_relative_size),
        static_cast<float>(dimensions.x - middle_rect_relative_size),
        static_cast<float>(dimensions.y - middle_rect_relative_size)
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &middleRect);

    if (value)
    {
        SDL_FRect innerRect{
            static_cast<float>(screenPosition.x + 0.5f*(middle_rect_relative_size + inner_rect_relative_size)),
            static_cast<float>(screenPosition.y + 0.5f*(middle_rect_relative_size + inner_rect_relative_size)),
            static_cast<float>(dimensions.x - (middle_rect_relative_size + inner_rect_relative_size)),
            static_cast<float>(dimensions.y - (middle_rect_relative_size + inner_rect_relative_size))
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &innerRect);
    }
}

