//
// Created by timon on 11/24/25.
//

#include "GetMousePositionType.h"

GameEvent::MousePositionType GameEvent::getMousePositionType(const std::vector<GUIRect *> & elements, Vector2Float mouse_position) {
    for (auto element: elements) {
        if (element->is_inside(mouse_position))
            return UI;
    }
    return Game;
}