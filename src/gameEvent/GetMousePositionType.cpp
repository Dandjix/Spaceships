//
// Created by timon on 11/24/25.
//

#include "GetMousePositionType.h"
#include "math/Vectors.h"
#include "userInterface/GUIRect.h"

GameEvent::MousePositionType GameEvent::getMousePositionType(const std::vector<GUIRect *> &elements,
                                                             Vector2Float mouse_position) {
    if (getElementUnderMouse(elements, mouse_position) == nullptr)
        return Game;
    return UI;
}

GUIRect *GameEvent::getElementUnderMouse(const std::vector<GUIRect *> &elements, Vector2Float mouse_position) {
    //iterate in reverse cuz the elements that are last to render (on top) are those which are visually under the mouse
    for (unsigned i = elements.size(); i-- > 0;)
        if (elements[i]->is_inside(mouse_position))
            return elements[i];
    return nullptr;
}
