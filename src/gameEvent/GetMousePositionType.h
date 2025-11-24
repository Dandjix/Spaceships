//
// Created by timon on 11/24/25.
//

#pragma once
#include "MousePositionType.h"
#include "userInterface/GUIRect.h"


namespace GameEvent{
    MousePositionType getMousePositionType(const std::vector<GUIRect *> &elements, Vector2Float mouse_position);
}




